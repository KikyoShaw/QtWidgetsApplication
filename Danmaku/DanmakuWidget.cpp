#include "DanmakuWidget.h"

#include <QBrush>
#include <QElapsedTimer>
#include <QFont>
#include <QFontMetricsF>
#include <QHideEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QPaintEvent>
#include <QPointer>
#include <QRectF>
#include <QResizeEvent>
#include <QShowEvent>
#include <QTimer>

#include <algorithm>
#include <cmath>
#include <deque>
#include <limits>
#include <utility>
#include <vector>

namespace
{
constexpr qreal ItemGap = 24.0;
constexpr qreal OutlineWidth = 2.0;
constexpr int FrameInterval = 16;

int boundedCount(std::size_t count)
{
    const std::size_t maximum = static_cast<std::size_t>((std::numeric_limits<int>::max)());
    return static_cast<int>((std::min)(count, maximum));
}

void incrementCount(int &count)
{
    if (count < (std::numeric_limits<int>::max)())
        ++count;
}
}

class DanmakuWidget::Private
{
public:
    struct Item
    {
        QString text;
        QColor color;
        quint64 sequence = 0;
        QPainterPath textPath;
        QPainterPath outlinePath;
        qreal width = 0.0;
        qreal height = 0.0;
        qreal x = 0.0;
        int lane = -1;
        bool geometryReady = false;
    };

    struct SettingsState
    {
        int fontSize;
        int requestedLanes;
        int effectiveLanes;
        qreal opacity;
        qreal speed;
        bool paused;

        bool operator!=(const SettingsState &other) const
        {
            return fontSize != other.fontSize || requestedLanes != other.requestedLanes
                || effectiveLanes != other.effectiveLanes || opacity != other.opacity
                || speed != other.speed || paused != other.paused;
        }
    };

    explicit Private(DanmakuWidget *widget)
        : q(widget), timer(new QTimer(widget))
    {
        timer->setInterval(FrameInterval);
        timer->setTimerType(Qt::PreciseTimer);
        rebuildFont();
    }

    QRectF contentRect() const
    {
        const QRect rect = q->contentsRect();
        return QRectF(rect.x(), rect.y(), (std::max)(0, rect.width()),
                      (std::max)(0, rect.height()));
    }

    void buildGeometry(Item &item)
    {
        QPainterPath textPath;
        textPath.addText(QPointF(0.0, 0.0), textFont, item.text);

        QPainterPathStroker stroker;
        stroker.setWidth(OutlineWidth);
        stroker.setCapStyle(Qt::RoundCap);
        stroker.setJoinStyle(Qt::RoundJoin);
        QPainterPath outlinePath = stroker.createStroke(textPath);
        const QRectF bounds = textPath.boundingRect().united(outlinePath.boundingRect());

        textPath.translate(-bounds.left(), -bounds.top());
        outlinePath.translate(-bounds.left(), -bounds.top());
        item.textPath = std::move(textPath);
        item.outlinePath = std::move(outlinePath);
        item.width = (std::max)(qreal(1.0), bounds.width());
        item.height = (std::max)(qreal(0.0), bounds.height());
        item.geometryReady = true;
    }

    void rebuildFont()
    {
        textFont = q->font();
        textFont.setPixelSize(fontSize);
        fontLineSpacing = QFontMetricsF(textFont, q).lineSpacing();
        for (Item &item : active)
            buildGeometry(item);
        if (!pending.empty())
            pending.front().geometryReady = false;
    }

    void prepareCandidate()
    {
        if (!pending.empty() && !pending.front().geometryReady)
            buildGeometry(pending.front());
    }

    void repackLanes()
    {
        if (effectiveLanes == 0)
            return;

        std::vector<std::vector<Item *>> lanes(static_cast<std::size_t>(effectiveLanes));
        std::vector<Item *> overflow;
        for (Item &item : active)
        {
            if (item.lane >= 0 && item.lane < effectiveLanes)
                lanes[static_cast<std::size_t>(item.lane)].push_back(&item);
            else
                overflow.push_back(&item);
        }

        const qreal emptyTail = -(std::numeric_limits<qreal>::infinity)();
        std::vector<qreal> tails(static_cast<std::size_t>(effectiveLanes), emptyTail);
        for (int lane = 0; lane < effectiveLanes; ++lane)
        {
            std::vector<Item *> &items = lanes[static_cast<std::size_t>(lane)];
            std::sort(items.begin(), items.end(), [](const Item *left, const Item *right) {
                if (left->x != right->x)
                    return left->x < right->x;
                return left->sequence < right->sequence;
            });
            qreal tail = emptyTail;
            for (Item *item : items)
            {
                item->x = (std::max)(item->x, tail + ItemGap);
                tail = item->x + item->width;
            }
            tails[static_cast<std::size_t>(lane)] = tail;
        }

        std::sort(overflow.begin(), overflow.end(), [](const Item *left, const Item *right) {
            return left->sequence < right->sequence;
        });
        for (Item *item : overflow)
        {
            const auto shortest = std::min_element(tails.begin(), tails.end());
            const int lane = static_cast<int>(shortest - tails.begin());
            item->lane = lane;
            item->x = (std::max)(item->x, *shortest + ItemGap);
            *shortest = item->x + item->width;
        }
        nextLane %= effectiveLanes;
    }

    void updateLayout(bool force)
    {
        prepareCandidate();
        qreal maximumHeight = 0.0;
        for (const Item &item : active)
            maximumHeight = (std::max)(maximumHeight, item.height);
        if (!pending.empty())
            maximumHeight = (std::max)(maximumHeight, pending.front().height);

        const qreal newLaneHeight = (std::max)(fontLineSpacing, maximumHeight) + ItemGap;
        const int fittingLanes = static_cast<int>(std::floor(contentRect().height() / newLaneHeight));
        const int newEffectiveLanes = (std::min)(requestedLanes, fittingLanes);
        const bool lanesChanged = newEffectiveLanes != effectiveLanes;
        const bool layoutChanged = force || lanesChanged || laneHeight != newLaneHeight;
        laneHeight = newLaneHeight;
        effectiveLanes = newEffectiveLanes;
        if (layoutChanged)
            repackLanes();
    }

    bool canAdvance() const
    {
        return displayed && !paused && effectiveLanes > 0 && !contentRect().isEmpty();
    }

    bool removeExpired()
    {
        const QRectF rect = contentRect();
        // A collapsed viewport freezes every active item, including its lane and x.
        if (effectiveLanes == 0 || rect.isEmpty())
            return false;
        const auto firstExpired = std::remove_if(active.begin(), active.end(), [&rect](const Item &item) {
            return item.x + item.width <= rect.left();
        });
        const bool changed = firstExpired != active.end();
        active.erase(firstExpired, active.end());
        return changed;
    }

    void dispatchQueued()
    {
        while (!pending.empty() && canAdvance())
        {
            // Only the FIFO head is measured; a taller candidate can reduce the lane count.
            updateLayout(false);
            if (!canAdvance())
                break;

            const qreal emptyTail = -(std::numeric_limits<qreal>::infinity)();
            std::vector<qreal> tails(static_cast<std::size_t>(effectiveLanes), emptyTail);
            for (const Item &item : active)
            {
                qreal &tail = tails[static_cast<std::size_t>(item.lane)];
                tail = (std::max)(tail, item.x + item.width);
            }

            const qreal entrance = contentRect().right();
            int selectedLane = -1;
            for (int offset = 0; offset < effectiveLanes; ++offset)
            {
                const int lane = (nextLane + offset) % effectiveLanes;
                if (tails[static_cast<std::size_t>(lane)] + ItemGap <= entrance)
                {
                    selectedLane = lane;
                    break;
                }
            }
            if (selectedLane < 0)
                break;

            Item item = std::move(pending.front());
            pending.pop_front();
            item.lane = selectedLane;
            item.x = entrance;
            active.push_back(std::move(item));
            nextLane = (selectedLane + 1) % effectiveLanes;
        }
    }

    qreal itemTop(const Item &item, const QRectF &rect) const
    {
        return rect.top() + item.lane * laneHeight + (laneHeight - item.height) / 2.0;
    }

    bool intersectsViewport(const Item &item, const QRectF &rect) const
    {
        if (effectiveLanes == 0 || item.lane < 0 || item.lane >= effectiveLanes)
            return false;
        return QRectF(item.x, itemTop(item, rect), item.width, item.height).intersects(rect);
    }

    void updateStatistics()
    {
        queued = boundedCount(pending.size());
        visible = 0;
        const QRectF rect = contentRect();
        for (const Item &item : active)
        {
            if (intersectsViewport(item, rect))
                incrementCount(visible);
            else
                incrementCount(queued);
        }
    }

    void updateTimer()
    {
        const bool needed = canAdvance() && (!active.empty() || !pending.empty());
        if (!needed)
        {
            timer->stop();
            clock.invalidate();
            lastFrameNsecs = 0;
        }
        else if (!timer->isActive())
        {
            // Hidden, paused and zero-lane time must never become animation time.
            clock.start();
            lastFrameNsecs = 0;
            timer->start();
        }
    }

    SettingsState currentSettings() const
    {
        return { fontSize, requestedLanes, effectiveLanes, textOpacity, speed, paused };
    }

    void notifyChanges()
    {
        if (notifying)
            return;
        notifying = true;
        QPointer<DanmakuWidget> owner(q);
        // Reentrant setters finish their mutations before the next notification.
        // No item pointers or iterators survive into a signal emission.
        while (currentSettings() != announcedSettings || queued != announcedQueued
               || visible != announcedVisible)
        {
            if (currentSettings() != announcedSettings)
            {
                announcedSettings = currentSettings();
                emit owner->settingsChanged();
                if (!owner)
                    return;
            }
            if (queued != announcedQueued || visible != announcedVisible)
            {
                announcedQueued = queued;
                announcedVisible = visible;
                emit owner->statisticsChanged();
                if (!owner)
                    return;
            }
        }
        notifying = false;
    }

    void finish(bool forceLayout = false)
    {
        updateLayout(forceLayout);
        if (removeExpired())
            updateLayout(false);
        dispatchQueued();
        updateTimer();
        updateStatistics();
        q->update();
        notifyChanges();
    }

    void tick()
    {
        if (canAdvance() && clock.isValid())
        {
            const qint64 now = clock.nsecsElapsed();
            const qreal seconds = static_cast<qreal>(now - lastFrameNsecs) / 1000000000.0;
            lastFrameNsecs = now;
            const qreal distance = speed * seconds;
            for (Item &item : active)
                item.x -= distance;
        }
        finish();
    }

    DanmakuWidget *q;
    QTimer *timer;
    QElapsedTimer clock;
    qint64 lastFrameNsecs = 0;
    QFont textFont;
    qreal fontLineSpacing = 0.0;
    int fontSize = 24;
    int requestedLanes = 6;
    int effectiveLanes = 0;
    qreal laneHeight = 0.0;
    qreal textOpacity = 0.85;
    qreal speed = 120.0;
    bool paused = false;
    bool displayed = false;
    quint64 nextSequence = 0;
    int nextLane = 0;
    std::deque<Item> pending;
    std::vector<Item> active;
    int queued = 0;
    int visible = 0;
    int announcedQueued = 0;
    int announcedVisible = 0;
    SettingsState announcedSettings { 24, 6, 0, 0.85, 120.0, false };
    bool notifying = false;
};

DanmakuWidget::DanmakuWidget(QWidget *parent)
    : QWidget(parent), d(new Private(this))
{
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TranslucentBackground);
    connect(d->timer, &QTimer::timeout, this, [this]() { d->tick(); });
    d->finish(true);
}

DanmakuWidget::~DanmakuWidget()
{
    d->timer->stop();
    QObject::disconnect(d->timer, nullptr, this, nullptr);
    d->clock.invalidate();
}

bool DanmakuWidget::sendText(const QString &text, const QColor &color)
{
    const QString singleLine = text.simplified();
    if (singleLine.isEmpty())
        return false;

    Private::Item item;
    item.text = singleLine;
    item.color = color.isValid() ? color : QColor(Qt::white);
    item.sequence = d->nextSequence++;
    d->pending.push_back(std::move(item));
    d->finish();
    return true;
}

int DanmakuWidget::fontSize() const
{
    return d->fontSize;
}

int DanmakuWidget::laneCount() const
{
    return d->requestedLanes;
}

int DanmakuWidget::effectiveLaneCount() const
{
    return d->effectiveLanes;
}

qreal DanmakuWidget::opacity() const
{
    return d->textOpacity;
}

qreal DanmakuWidget::scrollSpeed() const
{
    return d->speed;
}

bool DanmakuWidget::isPaused() const
{
    return d->paused;
}

int DanmakuWidget::queuedCount() const
{
    return d->queued;
}

int DanmakuWidget::visibleCount() const
{
    return d->visible;
}

void DanmakuWidget::setFontSize(int size)
{
    size = qBound(16, size, 48);
    if (d->fontSize == size)
        return;
    d->fontSize = size;
    d->rebuildFont();
    d->finish(true);
}

void DanmakuWidget::setLaneCount(int count)
{
    count = qBound(1, count, 12);
    if (d->requestedLanes == count)
        return;
    d->requestedLanes = count;
    d->finish(true);
}

void DanmakuWidget::setOpacity(qreal value)
{
    if (!std::isfinite(static_cast<double>(value)))
        return;
    value = qBound(qreal(0.0), value, qreal(1.0));
    if (d->textOpacity == value)
        return;
    d->textOpacity = value;
    d->finish();
}

void DanmakuWidget::setScrollSpeed(qreal pixelsPerSecond)
{
    if (!std::isfinite(static_cast<double>(pixelsPerSecond)))
        return;
    pixelsPerSecond = qBound(qreal(60.0), pixelsPerSecond, qreal(240.0));
    if (d->speed == pixelsPerSecond)
        return;
    d->speed = pixelsPerSecond;
    d->finish();
}

void DanmakuWidget::setPaused(bool paused)
{
    if (d->paused == paused)
        return;
    d->paused = paused;
    d->finish();
}

void DanmakuWidget::clear()
{
    d->pending.clear();
    d->active.clear();
    d->nextLane = 0;
    d->finish();
}

void DanmakuWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    const QRectF rect = d->contentRect();
    if (rect.isEmpty() || d->effectiveLanes == 0)
        return;

    QPainter painter(this);
    painter.save();
    painter.setClipRect(rect);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.setOpacity(d->textOpacity);
    const QBrush outlineBrush(Qt::black);
    for (const Private::Item &item : d->active)
    {
        if (!d->intersectsViewport(item, rect))
            continue;
        painter.save();
        painter.translate(item.x, d->itemTop(item, rect));
        painter.fillPath(item.outlinePath, outlineBrush);
        painter.fillPath(item.textPath, QBrush(item.color));
        painter.restore();
    }
    painter.restore();
}

void DanmakuWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    d->finish(true);
}

void DanmakuWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    d->displayed = true;
    d->finish(true);
}

void DanmakuWidget::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    d->displayed = false;
    d->finish();
}