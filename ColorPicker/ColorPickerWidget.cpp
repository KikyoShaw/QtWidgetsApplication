#include "ColorPickerWidget.h"

#include <QAbstractButton>
#include <QApplication>
#include <QClipboard>
#include <QFocusEvent>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QSet>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QVariant>
#include <QVector>
#include <functional>

namespace {

QString hexText(const QColor &color, bool includeAlpha)
{
    QString text = QStringLiteral("#%1%2%3")
        .arg(color.red(), 2, 16, QLatin1Char('0'))
        .arg(color.green(), 2, 16, QLatin1Char('0'))
        .arg(color.blue(), 2, 16, QLatin1Char('0'));
    if (includeAlpha)
        text += QStringLiteral("%1").arg(color.alpha(), 2, 16, QLatin1Char('0'));
    return text.toUpper();
}

bool parseHex(QString text, QColor &result)
{
    text = text.trimmed();
    if (text.startsWith(QLatin1Char('#')))
        text.remove(0, 1);
    if (text.size() != 6 && text.size() != 8)
        return false;

    for (const QChar ch : text) {
        const ushort c = ch.unicode();
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
              || (c >= 'A' && c <= 'F')))
            return false;
    }

    bool ok = false;
    const uint value = text.toUInt(&ok, 16);
    if (!ok)
        return false;
    if (text.size() == 6) {
        result = QColor((value >> 16) & 255, (value >> 8) & 255, value & 255, 255);
    } else {
        result = QColor((value >> 24) & 255, (value >> 16) & 255,
                        (value >> 8) & 255, value & 255);
    }
    return true;
}

void paintCheckerboard(QPainter &painter, const QRectF &area, int cellSize = 7)
{
    painter.save();
    painter.setClipRect(area, Qt::IntersectClip);
    painter.fillRect(area, QColor(QStringLiteral("#FFFFFF")));
    for (int row = 0; row * cellSize < area.height(); ++row) {
        for (int column = 0; column * cellSize < area.width(); ++column) {
            if ((row + column) % 2 == 0)
                painter.fillRect(QRectF(area.left() + column * cellSize,
                                        area.top() + row * cellSize,
                                        cellSize, cellSize),
                                 QColor(QStringLiteral("#E2E8F0")));
        }
    }
    painter.restore();
}

qreal normalizedPosition(qreal coordinate, qreal origin, qreal length)
{
    if (length <= 0.0)
        return 0.0;
    return qBound<qreal>(0.0, (coordinate - origin) / qMax<qreal>(1.0, length), 1.0);
}

// QWidget keeps the mouse grab until release, including dragging outside its bounds.
class InteractiveSurface : public QWidget
{
public:
    explicit InteractiveSurface(QWidget *parent) : QWidget(parent)
    {
        setFocusPolicy(Qt::StrongFocus);
        setCursor(Qt::CrossCursor);
    }

    std::function<void()> onFinished;

protected:
    QRectF drawingArea() const
    {
        return QRectF(rect()).adjusted(9.0, 7.0, -9.0, -7.0);
    }

    void paintFocus(QPainter &painter)
    {
        if (!hasFocus())
            return;
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor(QStringLiteral("#2563EB")), 1.5));
        painter.drawRoundedRect(QRectF(rect()).adjusted(1, 1, -1, -1), 7, 7);
    }

    virtual void changeAt(const QPointF &position) = 0;
    virtual void changeByKey(int key, bool fast) = 0;

    static bool isAdjustmentKey(int key)
    {
        return key == Qt::Key_Left || key == Qt::Key_Right
            || key == Qt::Key_Up || key == Qt::Key_Down;
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() != Qt::LeftButton) {
            QWidget::mousePressEvent(event);
            return;
        }
        setFocus(Qt::MouseFocusReason);
        m_dragging = true;
        changeAt(event->localPos());
        event->accept();
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        if (m_dragging) {
            changeAt(event->localPos());
            event->accept();
        } else {
            QWidget::mouseMoveEvent(event);
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton && m_dragging) {
            changeAt(event->localPos());
            m_dragging = false;
            finish();
            event->accept();
        } else {
            QWidget::mouseReleaseEvent(event);
        }
    }

    void keyPressEvent(QKeyEvent *event) override
    {
        if (!isAdjustmentKey(event->key())) {
            QWidget::keyPressEvent(event);
            return;
        }
        m_pressedKeys.insert(event->key());
        changeByKey(event->key(), event->modifiers().testFlag(Qt::ShiftModifier));
        event->accept();
    }

    void keyReleaseEvent(QKeyEvent *event) override
    {
        if (!isAdjustmentKey(event->key())) {
            QWidget::keyReleaseEvent(event);
            return;
        }
        if (!event->isAutoRepeat()) {
            const bool wasPressed = m_pressedKeys.remove(event->key()) > 0;
            if (wasPressed && m_pressedKeys.isEmpty() && !m_dragging)
                finish();
        }
        event->accept();
    }

    void focusInEvent(QFocusEvent *event) override
    {
        QWidget::focusInEvent(event);
        update();
    }

    void focusOutEvent(QFocusEvent *event) override
    {
        const bool editing = m_dragging || !m_pressedKeys.isEmpty();
        m_dragging = false;
        m_pressedKeys.clear();
        if (editing)
            finish();
        QWidget::focusOutEvent(event);
        update();
    }

private:
    void finish()
    {
        if (onFinished)
            onFinished();
    }

    bool m_dragging = false;
    QSet<int> m_pressedKeys;
};

class SaturationValuePanel : public InteractiveSurface
{
public:
    explicit SaturationValuePanel(QWidget *parent) : InteractiveSurface(parent)
    {
        setMinimumSize(200, 110);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setAccessibleName(QStringLiteral("饱和度与明度色板"));
        setToolTip(QStringLiteral("左右调整饱和度，上下调整明度；按住 Shift 加速"));
    }

    QSize sizeHint() const override { return QSize(500, 170); }

    void setState(qreal hue, qreal saturation, qreal value)
    {
        m_hue = hue;
        m_saturation = saturation;
        m_value = value;
        update();
    }

    std::function<void(qreal, qreal)> onChanged;

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF area = drawingArea();
        if (area.width() > 0 && area.height() > 0) {
            QPainterPath clip;
            clip.addRoundedRect(area, 7, 7);
            painter.save();
            painter.setClipPath(clip);
            QLinearGradient saturation(area.topLeft(), area.topRight());
            saturation.setColorAt(0, Qt::white);
            saturation.setColorAt(1, QColor::fromHsvF(m_hue == 1.0 ? 0.0 : m_hue, 1, 1));
            painter.fillRect(area, saturation);
            QLinearGradient value(area.topLeft(), area.bottomLeft());
            value.setColorAt(0, QColor(0, 0, 0, 0));
            value.setColorAt(1, Qt::black);
            painter.fillRect(area, value);
            painter.restore();

            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(QColor(0, 0, 0, 28), 1));
            painter.drawRoundedRect(area, 7, 7);
            const QPointF marker(area.left() + m_saturation * area.width(),
                                 area.top() + (1.0 - m_value) * area.height());
            painter.setPen(QPen(QColor(0, 0, 0, 170), 4));
            painter.drawEllipse(marker, 5, 5);
            painter.setPen(QPen(Qt::white, 2));
            painter.drawEllipse(marker, 5, 5);
        }
        paintFocus(painter);
    }

    void changeAt(const QPointF &position) override
    {
        const QRectF area = drawingArea();
        if (area.width() <= 0 || area.height() <= 0)
            return;
        publish(normalizedPosition(position.x(), area.left(), area.width()),
                1.0 - normalizedPosition(position.y(), area.top(), area.height()));
    }

    void changeByKey(int key, bool fast) override
    {
        const qreal step = fast ? 0.05 : 1.0 / 255.0;
        qreal saturation = m_saturation;
        qreal value = m_value;
        if (key == Qt::Key_Left) saturation -= step;
        if (key == Qt::Key_Right) saturation += step;
        if (key == Qt::Key_Up) value += step;
        if (key == Qt::Key_Down) value -= step;
        publish(saturation, value);
    }

private:
    void publish(qreal saturation, qreal value)
    {
        m_saturation = qBound<qreal>(0.0, saturation, 1.0);
        m_value = qBound<qreal>(0.0, value, 1.0);
        update();
        if (onChanged)
            onChanged(m_saturation, m_value);
    }

    qreal m_hue = 0;
    qreal m_saturation = 0;
    qreal m_value = 1;
};

class GradientBar : public InteractiveSurface
{
public:
    enum Kind { Hue, Alpha };

    GradientBar(Kind kind, QWidget *parent) : InteractiveSurface(parent), m_kind(kind)
    {
        setMinimumSize(100, 28);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setAccessibleName(kind == Hue ? QStringLiteral("色相") : QStringLiteral("透明度"));
        setToolTip(QStringLiteral("方向键微调，按住 Shift 加速"));
    }

    QSize sizeHint() const override { return QSize(400, 28); }

    void setState(qreal position, const QColor &color)
    {
        m_position = position;
        m_color = color;
        update();
    }

    std::function<void(qreal)> onChanged;

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF area = drawingArea();
        if (area.width() > 0 && area.height() > 0) {
            QPainterPath clip;
            clip.addRoundedRect(area, area.height() / 2, area.height() / 2);
            painter.save();
            painter.setClipPath(clip);
            QLinearGradient gradient(area.topLeft(), area.topRight());
            if (m_kind == Hue) {
                for (int i = 0; i <= 6; ++i)
                    gradient.setColorAt(i / 6.0, QColor::fromHsvF((i % 6) / 6.0, 1, 1));
            } else {
                paintCheckerboard(painter, area);
                QColor transparent = m_color;
                QColor opaque = m_color;
                transparent.setAlpha(0);
                opaque.setAlpha(255);
                gradient.setColorAt(0, transparent);
                gradient.setColorAt(1, opaque);
            }
            painter.fillRect(area, gradient);
            painter.restore();
            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen(QColor(0, 0, 0, 30), 1));
            painter.drawRoundedRect(area, area.height() / 2, area.height() / 2);

            const qreal x = area.left() + m_position * area.width();
            const QRectF marker(x - 3, area.top() - 3, 6, area.height() + 6);
            painter.setPen(QPen(QColor(QStringLiteral("#334155")), 1));
            painter.setBrush(Qt::white);
            painter.drawRoundedRect(marker, 3, 3);
        }
        paintFocus(painter);
    }

    void changeAt(const QPointF &position) override
    {
        const QRectF area = drawingArea();
        if (area.width() > 0 && area.height() > 0)
            publish(normalizedPosition(position.x(), area.left(), area.width()));
    }

    void changeByKey(int key, bool fast) override
    {
        const qreal step = m_kind == Hue ? (fast ? 10.0 : 1.0) / 360.0
                                         : (fast ? 10.0 : 1.0) / 255.0;
        const bool increase = key == Qt::Key_Right || key == Qt::Key_Up;
        publish(m_position + (increase ? step : -step));
    }

private:
    void publish(qreal position)
    {
        m_position = qBound<qreal>(0.0, position, 1.0);
        update();
        if (onChanged)
            onChanged(m_position);
    }

    Kind m_kind;
    qreal m_position = 0;
    QColor m_color = Qt::white;
};

class ColorPreview : public QWidget
{
public:
    explicit ColorPreview(QWidget *parent) : QWidget(parent)
    {
        setMinimumSize(76, 54);
        setMaximumWidth(92);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        setAccessibleName(QStringLiteral("当前颜色透明预览"));
    }

    void setColor(const QColor &color)
    {
        m_color = color;
        setToolTip(hexText(color, true));
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF area = QRectF(rect()).adjusted(1, 1, -1, -1);
        QPainterPath clip;
        clip.addRoundedRect(area, 8, 8);
        painter.save();
        painter.setClipPath(clip);
        paintCheckerboard(painter, area);
        painter.fillRect(area, m_color);
        painter.restore();
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QColor(QStringLiteral("#CBD5E1")));
        painter.drawRoundedRect(area, 8, 8);
    }

private:
    QColor m_color = Qt::white;
};

class ColorSwatch : public QAbstractButton
{
public:
    explicit ColorSwatch(QWidget *parent) : QAbstractButton(parent)
    {
        setFocusPolicy(Qt::StrongFocus);
        setCursor(Qt::PointingHandCursor);
        setMinimumSize(26, 28);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setEnabled(false);
    }

    QSize sizeHint() const override { return QSize(44, 28); }

    QColor color() const { return m_color; }

    void setColor(const QColor &color)
    {
        m_color = color;
        setEnabled(color.isValid());
        setToolTip(color.isValid() ? hexText(color, true) : QStringLiteral("暂无最近颜色"));
        setAccessibleName(color.isValid() ? QStringLiteral("选择颜色 %1").arg(hexText(color, true))
                                         : QStringLiteral("空的最近颜色槽"));
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF area = QRectF(rect()).adjusted(3, 3, -3, -3);
        QPainterPath clip;
        clip.addRoundedRect(area, 5, 5);
        painter.save();
        painter.setClipPath(clip);
        if (m_color.isValid()) {
            paintCheckerboard(painter, area, 5);
            painter.fillRect(area, m_color);
            if (isDown())
                painter.fillRect(area, QColor(0, 0, 0, 30));
        } else {
            painter.fillRect(area, QColor(QStringLiteral("#F1F5F9")));
        }
        painter.restore();
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QColor(QStringLiteral("#CBD5E1")), 1,
                            m_color.isValid() ? Qt::SolidLine : Qt::DashLine));
        painter.drawRoundedRect(area, 5, 5);
        if (hasFocus()) {
            painter.setPen(QPen(QColor(QStringLiteral("#2563EB")), 1.5));
            painter.drawRoundedRect(QRectF(rect()).adjusted(1, 1, -1, -1), 7, 7);
        }
    }

    void focusInEvent(QFocusEvent *event) override
    {
        QAbstractButton::focusInEvent(event);
        update();
    }

    void focusOutEvent(QFocusEvent *event) override
    {
        QAbstractButton::focusOutEvent(event);
        update();
    }

private:
    QColor m_color;
};

} // namespace

struct ColorPickerWidget::Private
{
    explicit Private(ColorPickerWidget *owner) : q(owner) {}

    void buildUi();
    void applyColor(const QColor &color);
    void synchronize();
    void rememberColor();
    void submitHex();
    void copyHex(bool includeAlpha);

    ColorPickerWidget *q;
    qreal hue = 0;
    qreal saturation = 0;
    qreal value = 1;
    qreal alpha = 1;
    QColor current;
    bool hexDirty = false;
    SaturationValuePanel *panel = nullptr;
    GradientBar *hueBar = nullptr;
    GradientBar *alphaBar = nullptr;
    ColorPreview *preview = nullptr;
    QLineEdit *hexEdit = nullptr;
    QLabel *hexHint = nullptr;
    QLabel *rgbaLabel = nullptr;
    QLabel *feedback = nullptr;
    QSpinBox *channels[4] = {};
    ColorSwatch *recentButtons[8] = {};
    QVector<QRgb> recentColors;
};

void ColorPickerWidget::Private::buildUi()
{
    q->setObjectName(QStringLiteral("ColorPickerWidget"));
    q->setAttribute(Qt::WA_StyledBackground, true);
    q->setStyleSheet(QStringLiteral(
        "QWidget#ColorPickerWidget { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 12px; }"
        "QWidget#ColorPickerWidget QLabel { color: #475569; font-size: 12px; }"
        "QWidget#ColorPickerWidget QLineEdit, QWidget#ColorPickerWidget QSpinBox {"
        " background: #F8FAFC; color: #0F172A; border: 1px solid #CBD5E1;"
        " border-radius: 5px; padding: 4px 6px; min-height: 18px; font-size: 12px; }"
        "QWidget#ColorPickerWidget QLineEdit:focus, QWidget#ColorPickerWidget QSpinBox:focus { border-color: #2563EB; }"
        "QWidget#ColorPickerWidget QPushButton { background: #EFF6FF; color: #1D4ED8;"
        " border: 1px solid #BFDBFE; border-radius: 5px; padding: 5px 9px; font-size: 12px; }"
        "QWidget#ColorPickerWidget QPushButton:hover { background: #DBEAFE; }"
        "QWidget#ColorPickerWidget QPushButton:pressed { background: #BFDBFE; }"
        "QWidget#ColorPickerWidget QPushButton:focus { border: 1px solid #2563EB; }"
        "QWidget#ColorPickerWidget QLabel#HexHint[invalid=\"true\"] { color: #B91C1C; }"
        "QWidget#ColorPickerWidget QLabel#CopyFeedback { color: #1D4ED8; }"));

    auto *layout = new QVBoxLayout(q);
    layout->setContentsMargins(14, 12, 14, 12);
    layout->setSpacing(7);

    panel = new SaturationValuePanel(q);
    layout->addWidget(panel, 1);
    auto *bars = new QGridLayout;
    bars->setContentsMargins(0, 0, 0, 0);
    bars->setHorizontalSpacing(8);
    bars->setVerticalSpacing(0);
    hueBar = new GradientBar(GradientBar::Hue, q);
    alphaBar = new GradientBar(GradientBar::Alpha, q);
    bars->addWidget(new QLabel(QStringLiteral("色相"), q), 0, 0);
    bars->addWidget(hueBar, 0, 1);
    bars->addWidget(new QLabel(QStringLiteral("透明度"), q), 1, 0);
    bars->addWidget(alphaBar, 1, 1);
    bars->setColumnStretch(1, 1);
    layout->addLayout(bars);

    auto *colorRow = new QHBoxLayout;
    colorRow->setSpacing(12);
    preview = new ColorPreview(q);
    colorRow->addWidget(preview);
    auto *editColumn = new QVBoxLayout;
    editColumn->setSpacing(4);
    auto *hexRow = new QHBoxLayout;
    hexRow->setSpacing(6);
    hexRow->addWidget(new QLabel(QStringLiteral("HEX"), q));
    hexEdit = new QLineEdit(q);
    hexEdit->setAccessibleName(QStringLiteral("HEX 颜色，RRGGBB 或 RRGGBBAA"));
    hexEdit->setPlaceholderText(QStringLiteral("#RRGGBB / #RRGGBBAA"));
    hexEdit->setMinimumWidth(120);
    hexEdit->setMaxLength(32);
    hexRow->addWidget(hexEdit, 1);
    auto *copySix = new QPushButton(QStringLiteral("复制 6 位"), q);
    auto *copyEight = new QPushButton(QStringLiteral("复制 8 位"), q);
    copySix->setToolTip(QStringLiteral("复制 #RRGGBB，不含透明度"));
    copyEight->setToolTip(QStringLiteral("复制 #RRGGBBAA，透明度在末尾"));
    hexRow->addWidget(copySix);
    hexRow->addWidget(copyEight);
    editColumn->addLayout(hexRow);
    hexHint = new QLabel(QStringLiteral("可省略 #；8 位格式的最后两位为透明度。"), q);
    hexHint->setObjectName(QStringLiteral("HexHint"));
    hexHint->setProperty("invalid", false);
    hexHint->setWordWrap(true);
    editColumn->addWidget(hexHint);
    rgbaLabel = new QLabel(q);
    rgbaLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    editColumn->addWidget(rgbaLabel);
    colorRow->addLayout(editColumn, 1);
    layout->addLayout(colorRow);

    auto *channelGrid = new QGridLayout;
    channelGrid->setHorizontalSpacing(10);
    channelGrid->setVerticalSpacing(3);
    const QString names[] = { QStringLiteral("R · 红"), QStringLiteral("G · 绿"),
                              QStringLiteral("B · 蓝"), QStringLiteral("A · 透明度") };
    for (int i = 0; i < 4; ++i) {
        auto *label = new QLabel(names[i], q);
        channels[i] = new QSpinBox(q);
        channels[i]->setRange(0, 255);
        channels[i]->setAccelerated(true);
        channels[i]->setAccessibleName(names[i]);
        label->setBuddy(channels[i]);
        channelGrid->addWidget(label, 0, i);
        channelGrid->addWidget(channels[i], 1, i);
        channelGrid->setColumnStretch(i, 1);
        QObject::connect(channels[i], QOverload<int>::of(&QSpinBox::valueChanged), q,
                         [this, i](int number) {
            if (i == 3) {
                // Alpha edits must not round-trip through RGB and erase hidden HSV state.
                alpha = number / 255.0;
                synchronize();
            } else {
                QColor color = current;
                if (i == 0) color.setRed(number);
                if (i == 1) color.setGreen(number);
                if (i == 2) color.setBlue(number);
                applyColor(color);
            }
        });
        QObject::connect(channels[i], &QSpinBox::editingFinished, q, [this] { rememberColor(); });
    }
    layout->addLayout(channelGrid);

    const QColor presets[] = { QColor(QStringLiteral("#EF4444")), QColor(QStringLiteral("#F97316")),
                               QColor(QStringLiteral("#FACC15")), QColor(QStringLiteral("#22C55E")),
                               QColor(QStringLiteral("#06B6D4")), QColor(QStringLiteral("#3B82F6")),
                               QColor(QStringLiteral("#A855F7")), QColor(QStringLiteral("#111827")) };
    auto *presetRow = new QHBoxLayout;
    auto *recentRow = new QHBoxLayout;
    presetRow->setSpacing(6);
    recentRow->setSpacing(6);
    auto *presetLabel = new QLabel(QStringLiteral("预置"), q);
    auto *recentLabel = new QLabel(QStringLiteral("最近"), q);
    presetLabel->setMinimumWidth(40);
    recentLabel->setMinimumWidth(40);
    presetRow->addWidget(presetLabel);
    recentRow->addWidget(recentLabel);
    QVector<QWidget *> tabOrder = { panel, hueBar, alphaBar, hexEdit, copySix, copyEight,
                                   channels[0], channels[1], channels[2], channels[3] };
    for (int i = 0; i < 8; ++i) {
        auto *preset = new ColorSwatch(q);
        preset->setColor(presets[i]);
        presetRow->addWidget(preset, 1);
        tabOrder.append(preset);
        QObject::connect(preset, &QAbstractButton::clicked, q, [this, preset] {
            const QColor color = preset->color();
            applyColor(color);
            rememberColor();
        });
        recentButtons[i] = new ColorSwatch(q);
        recentButtons[i]->setColor(QColor());
        recentRow->addWidget(recentButtons[i], 1);
        ColorSwatch *recent = recentButtons[i];
        QObject::connect(recent, &QAbstractButton::clicked, q, [this, recent] {
            const QColor color = recent->color();
            if (color.isValid()) {
                applyColor(color);
                rememberColor();
            }
        });
    }
    for (ColorSwatch *recent : recentButtons)
        tabOrder.append(recent);
    layout->addLayout(presetRow);
    layout->addLayout(recentRow);
    feedback = new QLabel(QStringLiteral("最近颜色在一次操作结束后收录，最多 8 个。"), q);
    feedback->setObjectName(QStringLiteral("CopyFeedback"));
    feedback->setWordWrap(true);
    layout->addWidget(feedback);
    for (int i = 1; i < tabOrder.size(); ++i)
        QWidget::setTabOrder(tabOrder[i - 1], tabOrder[i]);

    panel->onChanged = [this](qreal s, qreal v) {
        saturation = s;
        value = v;
        synchronize();
    };
    hueBar->onChanged = [this](qreal h) {
        hue = h;
        synchronize();
    };
    alphaBar->onChanged = [this](qreal a) {
        alpha = a;
        synchronize();
    };
    panel->onFinished = [this] { rememberColor(); };
    hueBar->onFinished = [this] { rememberColor(); };
    alphaBar->onFinished = [this] { rememberColor(); };
    QObject::connect(hexEdit, &QLineEdit::textEdited, q, [this] {
        hexDirty = true;
        hexHint->setProperty("invalid", false);
        hexHint->setText(QStringLiteral("按 Enter 或移出输入框提交；8 位格式为 #RRGGBBAA。"));
        hexHint->setStyleSheet(QString());
    });
    QObject::connect(hexEdit, &QLineEdit::editingFinished, q, [this] { submitHex(); });
    QObject::connect(copySix, &QPushButton::clicked, q, [this] { copyHex(false); });
    QObject::connect(copyEight, &QPushButton::clicked, q, [this] { copyHex(true); });
}

void ColorPickerWidget::Private::applyColor(const QColor &color)
{
    if (!color.isValid())
        return;
    qreal newHue = 0;
    qreal newSaturation = 0;
    qreal newValue = 0;
    qreal newAlpha = 0;
    color.toRgb().getHsvF(&newHue, &newSaturation, &newValue, &newAlpha);
    if (newHue >= 0)
        hue = newHue;
    // RGB cannot encode hue at gray, or saturation at black. Keep those editing axes.
    if (newValue > 0)
        saturation = newSaturation;
    value = newValue;
    alpha = newAlpha;
    synchronize();
}

void ColorPickerWidget::Private::synchronize()
{
    const QColor next = QColor::fromRgba(
        QColor::fromHsvF(hue == 1.0 ? 0.0 : hue, saturation, value, alpha).rgba());
    const bool changed = !current.isValid() || current.rgba() != next.rgba();
    current = next;
    panel->setState(hue, saturation, value);
    hueBar->setState(hue, current);
    alphaBar->setState(alpha, current);
    preview->setColor(current);
    if (!hexDirty)
        hexEdit->setText(hexText(current, current.alpha() != 255));
    rgbaLabel->setText(QStringLiteral("RGBA  %1, %2, %3, %4")
                       .arg(current.red()).arg(current.green()).arg(current.blue()).arg(current.alpha()));
    const int values[] = { current.red(), current.green(), current.blue(), current.alpha() };
    for (int i = 0; i < 4; ++i) {
        const QSignalBlocker blocker(channels[i]);
        channels[i]->setValue(values[i]);
    }
    if (changed)
        emit q->colorChanged(next);
}

void ColorPickerWidget::Private::rememberColor()
{
    if (!current.isValid())
        return;
    recentColors.removeAll(current.rgba());
    recentColors.prepend(current.rgba());
    if (recentColors.size() > 8)
        recentColors.resize(8);
    // Reuse the buttons: a clicked sender must remain alive while its slot runs.
    for (int i = 0; i < 8; ++i)
        recentButtons[i]->setColor(i < recentColors.size()
                                  ? QColor::fromRgba(recentColors[i]) : QColor());
}

void ColorPickerWidget::Private::submitHex()
{
    if (!hexDirty)
        return;
    QColor parsed;
    const bool valid = parseHex(hexEdit->text(), parsed);
    hexDirty = false;
    if (valid) {
        applyColor(parsed);
        rememberColor();
        hexHint->setProperty("invalid", false);
        hexHint->setStyleSheet(QString());
        hexHint->setText(QStringLiteral("可省略 #；8 位格式的最后两位为透明度。"));
    } else {
        hexEdit->setText(hexText(current, current.alpha() != 255));
        hexHint->setProperty("invalid", true);
        hexHint->setStyleSheet(QStringLiteral("color: #B91C1C;"));
        hexHint->setText(QStringLiteral("格式无效，已恢复当前颜色。请输入 6 或 8 位十六进制值。"));
    }
}

void ColorPickerWidget::Private::copyHex(bool includeAlpha)
{
    const QString text = hexText(current, includeAlpha);
    QApplication::clipboard()->setText(text);
    feedback->setText(QStringLiteral("已复制 %1（%2）")
                      .arg(text, includeAlpha ? QStringLiteral("RRGGBBAA") : QStringLiteral("RRGGBB")));
}

ColorPickerWidget::ColorPickerWidget(QWidget *parent)
    : QWidget(parent), d(new Private(this))
{
    d->buildUi();
    d->applyColor(QColor(QStringLiteral("#3B82F6")));
}

ColorPickerWidget::~ColorPickerWidget()
{
    d->panel->onChanged = {};
    d->panel->onFinished = {};
    d->hueBar->onChanged = {};
    d->hueBar->onFinished = {};
    d->alphaBar->onChanged = {};
    d->alphaBar->onFinished = {};
    for (QObject *child : children())
        QObject::disconnect(child, nullptr, this, nullptr);
}

QColor ColorPickerWidget::selectedColor() const
{
    return d->current;
}

void ColorPickerWidget::setSelectedColor(const QColor &color)
{
    d->applyColor(color);
}