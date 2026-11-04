#include "NodeEditorWidget.h"

#include <QApplication>
#include <QBrush>
#include <QColor>
#include <QCursor>
#include <QEvent>
#include <QFocusEvent>
#include <QFont>
#include <QFontMetricsF>
#include <QFrame>
#include <QGraphicsItem>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonValue>
#include <QKeyEvent>
#include <QKeySequence>
#include <QLineEdit>
#include <QLineF>
#include <QList>
#include <QMap>
#include <QMetaObject>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QPen>
#include <QPoint>
#include <QPointF>
#include <QPointer>
#include <QRect>
#include <QRectF>
#include <QScopedValueRollback>
#include <QScrollBar>
#include <QSet>
#include <QShowEvent>
#include <QTimer>
#include <QTransform>
#include <QUuid>
#include <QVariant>
#include <QVBoxLayout>
#include <QVector>
#include <QWheelEvent>

#include <cmath>
#include <functional>
#include <utility>

namespace {

const qreal CoordinateLimit = 1000000.0;
const qreal MinimumZoom = 0.1;
const qreal MaximumZoom = 4.0;
const qreal NodeWidth = 200.0;
const qreal NodeHeight = 126.0;

QRectF nodeBounds()
{
    // 包含边框和伸出节点主体的端口命中区域。
    return QRectF(-10.0, -2.0, NodeWidth + 20.0, NodeHeight + 4.0);
}

QRectF canvasBounds()
{
    return QRectF(-CoordinateLimit, -CoordinateLimit,
                  CoordinateLimit * 2.0, CoordinateLimit * 2.0);
}

bool validKind(NodeEditorWidget::Kind kind)
{
    return kind == NodeEditorWidget::Input || kind == NodeEditorWidget::Process
        || kind == NodeEditorWidget::Output;
}

QString kindName(NodeEditorWidget::Kind kind)
{
    switch (kind) {
    case NodeEditorWidget::Input: return QStringLiteral("input");
    case NodeEditorWidget::Process: return QStringLiteral("process");
    case NodeEditorWidget::Output: return QStringLiteral("output");
    default: return QString();
    }
}

bool parseKind(const QString &name, NodeEditorWidget::Kind *kind)
{
    if (name == QStringLiteral("input"))
        *kind = NodeEditorWidget::Input;
    else if (name == QStringLiteral("process"))
        *kind = NodeEditorWidget::Process;
    else if (name == QStringLiteral("output"))
        *kind = NodeEditorWidget::Output;
    else
        return false;
    return true;
}

QString defaultTitle(NodeEditorWidget::Kind kind)
{
    switch (kind) {
    case NodeEditorWidget::Input: return QStringLiteral("数据输入");
    case NodeEditorWidget::Process: return QStringLiteral("数据处理");
    case NodeEditorWidget::Output: return QStringLiteral("输出结果");
    default: return QString();
    }
}

QColor kindColor(NodeEditorWidget::Kind kind)
{
    switch (kind) {
    case NodeEditorWidget::Input: return QColor(53, 112, 186);
    case NodeEditorWidget::Process: return QColor(118, 83, 173);
    case NodeEditorWidget::Output: return QColor(43, 132, 110);
    default: return QColor(80, 90, 110);
    }
}

bool portDirection(NodeEditorWidget::Kind kind, const QString &port, bool *isOutput)
{
    if ((kind == NodeEditorWidget::Input || kind == NodeEditorWidget::Process)
        && port == QStringLiteral("out")) {
        *isOutput = true;
        return true;
    }
    if ((kind == NodeEditorWidget::Process
         && (port == QStringLiteral("a") || port == QStringLiteral("b")))
        || (kind == NodeEditorWidget::Output && port == QStringLiteral("in"))) {
        *isOutput = false;
        return true;
    }
    return false;
}

bool validPosition(const QPointF &position)
{
    if (!std::isfinite(position.x()) || !std::isfinite(position.y()))
        return false;
    const QRectF bounds = nodeBounds().translated(position);
    return bounds.left() >= -CoordinateLimit && bounds.right() <= CoordinateLimit
        && bounds.top() >= -CoordinateLimit && bounds.bottom() <= CoordinateLimit;
}

QPointF boundedPosition(const QPointF &position)
{
    const QRectF bounds = nodeBounds();
    return QPointF(qBound(-CoordinateLimit - bounds.left(), position.x(),
                          CoordinateLimit - bounds.right()),
                   qBound(-CoordinateLimit - bounds.top(), position.y(),
                          CoordinateLimit - bounds.bottom()));
}

QPainterPath connectionPath(const QPointF &output, const QPointF &input)
{
    const qreal reach = qMax(qreal(60.0), std::abs(input.x() - output.x()) * 0.5);
    QPainterPath path(output);
    path.cubicTo(output + QPointF(reach, 0.0), input - QPointF(reach, 0.0), input);
    return path;
}

class NodeItem;
class EdgeItem;

class PortItem : public QGraphicsItem
{
public:
    enum { Type = QGraphicsItem::UserType + 2 };

    PortItem(NodeItem *node, const QString &id, bool output, const QPointF &position);

    int type() const override { return Type; }
    QRectF boundingRect() const override { return QRectF(-10.0, -10.0, 20.0, 20.0); }
    QPainterPath shape() const override
    {
        QPainterPath result;
        result.addEllipse(QRectF(-9.0, -9.0, 18.0, 18.0));
        return result;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    NodeItem *node() const { return m_node; }
    const QString &id() const { return m_id; }
    bool isOutput() const { return m_output; }

private:
    NodeItem *m_node;
    QString m_id;
    bool m_output;
};

class NodeItem : public QGraphicsItem
{
public:
    enum { Type = QGraphicsItem::UserType + 1 };

    NodeItem(const QString &id, NodeEditorWidget::Kind kind, const QString &title)
        : m_id(id), m_kind(kind), m_title(title)
    {
        setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
        setZValue(1.0);
        setToolTip(title);
        if (kind == NodeEditorWidget::Process) {
            addPort(QStringLiteral("a"), false, QPointF(0.0, 65.0));
            addPort(QStringLiteral("b"), false, QPointF(0.0, 101.0));
        } else if (kind == NodeEditorWidget::Output) {
            addPort(QStringLiteral("in"), false, QPointF(0.0, 83.0));
        }
        if (kind != NodeEditorWidget::Output)
            addPort(QStringLiteral("out"), true, QPointF(NodeWidth, 83.0));
    }

    int type() const override { return Type; }
    QRectF boundingRect() const override { return nodeBounds(); }
    QPainterPath shape() const override
    {
        QPainterPath result;
        result.addRoundedRect(QRectF(0.0, 0.0, NodeWidth, NodeHeight), 10.0, 10.0);
        return result;
    }

    const QString &id() const { return m_id; }
    NodeEditorWidget::Kind kind() const { return m_kind; }
    const QString &title() const { return m_title; }
    PortItem *port(const QString &id) const { return m_ports.value(id, nullptr); }

    void setTitle(const QString &title)
    {
        m_title = title;
        setToolTip(title);
        update();
    }

    void clearCallbacks()
    {
        positionChanged = nullptr;
        renameRequested = nullptr;
    }

    QSet<EdgeItem *> incidentEdges;
    std::function<void(NodeItem *)> positionChanged;
    std::function<void(const QString &, const QString &)> renameRequested;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        const QPainterPath outline = shape();
        painter->fillPath(outline, QColor(39, 44, 55));
        painter->save();
        painter->setClipPath(outline);
        painter->fillRect(QRectF(0.0, 0.0, NodeWidth, 42.0), kindColor(m_kind));
        painter->restore();

        QPen border(isSelected() ? QColor(125, 198, 255) : QColor(79, 88, 106));
        border.setWidthF(isSelected() ? 2.2 : 1.0);
        painter->setPen(border);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(outline);

        QFont titleFont = painter->font();
        titleFont.setPointSizeF(10.0);
        titleFont.setBold(true);
        painter->setFont(titleFont);
        painter->setPen(QColor(249, 250, 253));
        const QFontMetricsF metrics(titleFont);
        const QString shortTitle = metrics.elidedText(m_title, Qt::ElideRight, NodeWidth - 28.0);
        painter->drawText(QRectF(14.0, 0.0, NodeWidth - 28.0, 42.0),
                          Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, shortTitle);

        QFont labelFont = titleFont;
        labelFont.setPointSizeF(9.0);
        labelFont.setBold(false);
        painter->setFont(labelFont);
        painter->setPen(QColor(210, 218, 232));
        if (m_kind == NodeEditorWidget::Process) {
            painter->drawText(QRectF(16.0, 54.0, 82.0, 22.0), Qt::AlignVCenter,
                              QStringLiteral("输入 A"));
            painter->drawText(QRectF(16.0, 90.0, 82.0, 22.0), Qt::AlignVCenter,
                              QStringLiteral("输入 B"));
        } else if (m_kind == NodeEditorWidget::Output) {
            painter->drawText(QRectF(16.0, 72.0, 82.0, 22.0), Qt::AlignVCenter,
                              QStringLiteral("输入"));
        }
        if (m_kind != NodeEditorWidget::Output) {
            painter->drawText(QRectF(106.0, 72.0, 78.0, 22.0),
                              Qt::AlignRight | Qt::AlignVCenter, QStringLiteral("输出"));
        }
    }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override
    {
        if (change == ItemPositionChange) {
            const QPointF requested = value.toPointF();
            if (!std::isfinite(requested.x()) || !std::isfinite(requested.y()))
                return pos();
            // 返回约束位置，不在 itemChange 内再次 setPos。
            return boundedPosition(requested);
        }
        const QVariant result = QGraphicsItem::itemChange(change, value);
        if (change == ItemPositionHasChanged && positionChanged)
            positionChanged(this);
        return result;
    }

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            event->accept();
            const QString nodeId = m_id;
            const QString oldTitle = m_title;
            const auto callback = renameRequested;
            if (callback)
                callback(nodeId, oldTitle);
            return;
        }
        QGraphicsItem::mouseDoubleClickEvent(event);
    }

private:
    void addPort(const QString &id, bool output, const QPointF &position)
    {
        m_ports.insert(id, new PortItem(this, id, output, position));
    }

    QString m_id;
    NodeEditorWidget::Kind m_kind;
    QString m_title;
    QMap<QString, PortItem *> m_ports;
};

PortItem::PortItem(NodeItem *node, const QString &id, bool output, const QPointF &position)
    : QGraphicsItem(node), m_node(node), m_id(id), m_output(output)
{
    setPos(position);
    setZValue(2.0);
    setAcceptedMouseButtons(Qt::NoButton);
    setToolTip((output ? QStringLiteral("输出端口：") : QStringLiteral("输入端口：")) + id);
}

void PortItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen border(QColor(223, 234, 251));
    border.setWidthF(1.4);
    painter->setPen(border);
    painter->setBrush(kindColor(m_node->kind()).lighter(135));
    painter->drawEllipse(QRectF(-5.5, -5.5, 11.0, 11.0));
}

class EdgeItem : public QGraphicsPathItem
{
public:
    enum { Type = QGraphicsItem::UserType + 3 };

    EdgeItem(const QString &id, PortItem *output, PortItem *input)
        : m_id(id), m_output(output), m_input(input)
    {
        setFlag(ItemIsSelectable, true);
        setZValue(-1.0);
        QPen pen(QColor(137, 156, 188));
        pen.setWidthF(2.4);
        pen.setCosmetic(true);
        setPen(pen);
        setBrush(Qt::NoBrush);
        updateEndpoints();
    }

    int type() const override { return Type; }
    const QString &id() const { return m_id; }
    PortItem *output() const { return m_output; }
    PortItem *input() const { return m_input; }

    QPainterPath shape() const override
    {
        QPainterPathStroker stroker;
        stroker.setWidth(12.0);
        stroker.setCapStyle(Qt::RoundCap);
        return stroker.createStroke(path());
    }

    QRectF boundingRect() const override
    {
        // 同时覆盖宽命中轮廓和最小缩放下的 cosmetic 画笔。
        return shape().boundingRect().adjusted(-8.0, -8.0, 8.0, 8.0);
    }

    void updateEndpoints()
    {
        setPath(connectionPath(m_output->scenePos(), m_input->scenePos()));
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override
    {
        painter->setRenderHint(QPainter::Antialiasing, true);
        QPen line(isSelected() ? QColor(125, 206, 255) : QColor(137, 156, 188));
        line.setWidthF(isSelected() ? 2.4 : 1.8);
        line.setCosmetic(true);
        line.setCapStyle(Qt::RoundCap);
        painter->setPen(line);
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(path());
    }

private:
    QString m_id;
    PortItem *m_output;
    PortItem *m_input;
};

class NodeScene : public QGraphicsScene
{
public:
    explicit NodeScene(QObject *parent) : QGraphicsScene(parent)
    {
        setSceneRect(canvasBounds());
        setBackgroundBrush(QColor(24, 28, 36));
    }
};

class NodeView : public QGraphicsView
{
public:
    explicit NodeView(QWidget *parent) : QGraphicsView(parent)
    {
        setFrameShape(QFrame::NoFrame);
        setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        setDragMode(QGraphicsView::RubberBandDrag);
        setRubberBandSelectionMode(Qt::IntersectsItemShape);
        setTransformationAnchor(QGraphicsView::NoAnchor);
        setResizeAnchor(QGraphicsView::AnchorViewCenter);
        setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);
        setBackgroundBrush(QColor(24, 28, 36));
    }

    ~NodeView() override
    {
        cancelInteraction(false);
    }

    std::function<void(PortItem *, PortItem *)> connectionRequested;
    std::function<void()> deleteRequested;
    std::function<void()> selectAllRequested;
    std::function<void(qreal)> scaleChanged;

    qreal zoomFactor() const { return m_scale; }

    void setZoomAt(qreal factor, const QPoint &anchor)
    {
        factor = qBound(MinimumZoom, factor, MaximumZoom);
        if (qFuzzyCompare(factor, m_scale))
            return;
        const QPointF previousAnchor = mapToScene(anchor);
        const QPointF previousCenter = mapToScene(viewport()->rect().center());
        setTransform(QTransform::fromScale(factor, factor));
        centerOn(previousCenter);
        const QPointF shiftedAnchor = mapToScene(anchor);
        const QPointF center = mapToScene(viewport()->rect().center());
        centerOn(center + previousAnchor - shiftedAnchor);
        m_scale = factor;
        viewport()->update();
        if (scaleChanged)
            scaleChanged(m_scale);
    }

    void fitGraph(const QRectF &bounds)
    {
        cancelInteraction(true);
        const QRectF padded = bounds.adjusted(-32.0, -32.0, 32.0, 32.0);
        const qreal availableWidth = qMax(1, viewport()->width() - 24);
        const qreal availableHeight = qMax(1, viewport()->height() - 24);
        const qreal factor = qBound(MinimumZoom,
                                   qMin(availableWidth / padded.width(),
                                        availableHeight / padded.height()), MaximumZoom);
        const bool changed = !qFuzzyCompare(factor, m_scale);
        setTransform(QTransform::fromScale(factor, factor));
        centerOn(padded.center());
        m_scale = factor;
        viewport()->update();
        if (changed && scaleChanged)
            scaleChanged(m_scale);
    }

    void cancelInteraction(bool suppressCurrentButtons)
    {
        if (m_cancelling)
            return;
        QScopedValueRollback<bool> guard(m_cancelling, true);
        cancelWire();
        m_panning = false;
        m_panButton = Qt::NoButton;
        m_spaceDown = false;

        // 让 QGraphicsView 结束其私有的拖动/框选状态，再释放场景抓取者。
        const Qt::MouseButtons forwarded = m_forwardedButtons;
        m_forwardedButtons = Qt::NoButton;
        const Qt::MouseButton buttons[] = { Qt::LeftButton, Qt::RightButton, Qt::MiddleButton };
        for (Qt::MouseButton button : buttons) {
            if (forwarded.testFlag(button)) {
                QMouseEvent release(QEvent::MouseButtonRelease, QPointF(m_lastPosition),
                                    QPointF(viewport()->mapToGlobal(m_lastPosition)), button,
                                    Qt::NoButton, Qt::NoModifier);
                QGraphicsView::mouseReleaseEvent(&release);
            }
        }
        if (scene() && scene()->mouseGrabberItem())
            scene()->mouseGrabberItem()->ungrabMouse();
        if (QWidget::mouseGrabber() == viewport())
            viewport()->releaseMouse();
        m_suppressDrag = suppressCurrentButtons && QApplication::mouseButtons() != Qt::NoButton;
        updateCursor();
    }

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override
    {
        painter->fillRect(rect, QColor(24, 28, 36));
        qreal step = 24.0;
        while (step * m_scale < 18.0)
            step *= 2.0;
        const QRectF visible = rect.intersected(canvasBounds());
        if (visible.isEmpty())
            return;
        QVector<QLineF> minor;
        QVector<QLineF> major;
        const int estimate = int(visible.width() / step + visible.height() / step) + 4;
        minor.reserve(estimate);
        major.reserve(estimate / 5 + 4);
        const qreal firstX = std::floor(visible.left() / step) * step;
        const qreal firstY = std::floor(visible.top() / step) * step;
        for (qreal x = firstX; x <= visible.right(); x += step) {
            const QLineF line(x, visible.top(), x, visible.bottom());
            if (qRound64(x / step) % 5 == 0)
                major.append(line);
            else
                minor.append(line);
        }
        for (qreal y = firstY; y <= visible.bottom(); y += step) {
            const QLineF line(visible.left(), y, visible.right(), y);
            if (qRound64(y / step) % 5 == 0)
                major.append(line);
            else
                minor.append(line);
        }
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, false);
        QPen grid(QColor(33, 38, 48));
        grid.setCosmetic(true);
        painter->setPen(grid);
        if (!minor.isEmpty())
            painter->drawLines(minor.constData(), minor.size());
        grid.setColor(QColor(42, 48, 60));
        painter->setPen(grid);
        if (!major.isEmpty())
            painter->drawLines(major.constData(), major.size());
        painter->restore();
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        m_lastPosition = event->pos();
        m_suppressDrag = false;
        if (event->button() == Qt::MiddleButton
            || (event->button() == Qt::LeftButton && m_spaceDown)) {
            const bool keepSpace = m_spaceDown;
            cancelInteraction(false);
            m_spaceDown = keepSpace;
            setFocus(Qt::MouseFocusReason);
            m_panning = true;
            m_panButton = event->button();
            viewport()->grabMouse();
            updateCursor();
            event->accept();
            return;
        }
        if (m_panning || m_wireStart) {
            event->accept();
            return;
        }
        if (event->button() == Qt::LeftButton) {
            PortItem *port = portAt(event->pos());
            if (port && scene()) {
                cancelInteraction(false);
                setFocus(Qt::MouseFocusReason);
                m_wireStart = port;
                m_temporary = new QGraphicsPathItem;
                m_temporary->setAcceptedMouseButtons(Qt::NoButton);
                m_temporary->setZValue(-0.5);
                QPen line(QColor(125, 206, 255));
                line.setWidthF(2.0);
                line.setCosmetic(true);
                line.setStyle(Qt::DashLine);
                m_temporary->setPen(line);
                scene()->addItem(m_temporary);
                viewport()->grabMouse();
                updateWire(event->pos());
                updateCursor();
                event->accept();
                return;
            }
        }
        m_forwardedButtons |= event->button();
        QGraphicsView::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent *event) override
    {
        const QPoint delta = event->pos() - m_lastPosition;
        m_lastPosition = event->pos();
        if (m_suppressDrag) {
            if (event->buttons() == Qt::NoButton)
                m_suppressDrag = false;
            event->accept();
            return;
        }
        if (m_panning) {
            if (!event->buttons().testFlag(m_panButton)) {
                cancelInteraction(false);
            } else {
                horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
                verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
            }
            event->accept();
            return;
        }
        if (m_wireStart) {
            if (!event->buttons().testFlag(Qt::LeftButton))
                cancelInteraction(false);
            else
                updateWire(event->pos());
            event->accept();
            return;
        }
        QGraphicsView::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent *event) override
    {
        m_lastPosition = event->pos();
        if (m_suppressDrag) {
            if (event->buttons() == Qt::NoButton)
                m_suppressDrag = false;
            event->accept();
            return;
        }
        if (m_panning) {
            if (event->button() == m_panButton) {
                m_panning = false;
                m_panButton = Qt::NoButton;
                if (QWidget::mouseGrabber() == viewport())
                    viewport()->releaseMouse();
                m_suppressDrag = event->buttons() != Qt::NoButton;
                updateCursor();
            }
            event->accept();
            return;
        }
        if (m_wireStart) {
            event->accept();
            if (event->button() == Qt::LeftButton) {
                PortItem *start = m_wireStart;
                PortItem *finish = portAt(event->pos());
                cancelWire();
                if (QWidget::mouseGrabber() == viewport())
                    viewport()->releaseMouse();
                m_suppressDrag = event->buttons() != Qt::NoButton;
                updateCursor();
                if (finish && connectionRequested)
                    connectionRequested(start, finish);
            }
            return;
        }
        m_forwardedButtons &= ~Qt::MouseButtons(event->button());
        QGraphicsView::mouseReleaseEvent(event);
    }

    void mouseDoubleClickEvent(QMouseEvent *event) override
    {
        m_lastPosition = event->pos();
        if (m_panning || m_spaceDown || m_wireStart || portAt(event->pos())) {
            cancelInteraction(true);
            event->accept();
            return;
        }
        m_forwardedButtons |= event->button();
        QGraphicsView::mouseDoubleClickEvent(event);
    }

    void wheelEvent(QWheelEvent *event) override
    {
        const int angle = event->angleDelta().y();
        const int pixels = event->pixelDelta().y();
        if (angle != 0 || pixels != 0) {
            const qreal steps = qBound(qreal(-20.0),
                                      angle != 0 ? angle / 120.0 : pixels / 120.0,
                                      qreal(20.0));
            setZoomAt(m_scale * std::pow(1.15, steps), event->pos());
            if (m_wireStart)
                updateWire(event->pos());
        }
        event->accept();
    }

    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_Escape) {
            cancelInteraction(true);
            event->accept();
            return;
        }
        if (event->key() == Qt::Key_Space) {
            if (!event->isAutoRepeat()) {
                if (!m_forwardedButtons.testFlag(Qt::LeftButton) && !m_wireStart)
                    m_spaceDown = true;
                else {
                    cancelInteraction(true);
                    m_spaceDown = true;
                }
                updateCursor();
            }
            event->accept();
            return;
        }
        if (event->matches(QKeySequence::SelectAll)) {
            event->accept();
            if (selectAllRequested)
                selectAllRequested();
            return;
        }
        if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
            event->accept();
            if (deleteRequested)
                deleteRequested();
            return;
        }
        QGraphicsView::keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_Space) {
            if (!event->isAutoRepeat()) {
                m_spaceDown = false;
                updateCursor();
            }
            event->accept();
            return;
        }
        QGraphicsView::keyReleaseEvent(event);
    }

    void focusOutEvent(QFocusEvent *event) override
    {
        cancelInteraction(true);
        QGraphicsView::focusOutEvent(event);
    }

    bool event(QEvent *event) override
    {
        if (event->type() == QEvent::WindowDeactivate || event->type() == QEvent::Hide)
            cancelInteraction(true);
        return QGraphicsView::event(event);
    }

    bool viewportEvent(QEvent *event) override
    {
        if (event->type() == QEvent::WindowDeactivate || event->type() == QEvent::Hide
            || event->type() == QEvent::FocusOut)
            cancelInteraction(true);
        return QGraphicsView::viewportEvent(event);
    }

    void showEvent(QShowEvent *event) override
    {
        QWidget *topLevel = window();
        if (m_watchedWindow != topLevel) {
            if (m_watchedWindow)
                m_watchedWindow->removeEventFilter(this);
            m_watchedWindow = topLevel;
            if (topLevel != this)
                topLevel->installEventFilter(this);
        }
        QGraphicsView::showEvent(event);
    }

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched == m_watchedWindow.data()
            && (event->type() == QEvent::WindowDeactivate || event->type() == QEvent::Hide))
            cancelInteraction(true);
        return QGraphicsView::eventFilter(watched, event);
    }

private:
    PortItem *portAt(const QPoint &position) const
    {
        QGraphicsItem *item = itemAt(position);
        while (item) {
            if (item->type() == PortItem::Type)
                return static_cast<PortItem *>(item);
            item = item->parentItem();
        }
        return nullptr;
    }

    void updateWire(const QPoint &position)
    {
        if (!m_wireStart || !m_temporary)
            return;
        const QPointF fixed = m_wireStart->scenePos();
        const QPointF moving = mapToScene(position);
        m_temporary->setPath(m_wireStart->isOutput()
                                ? connectionPath(fixed, moving)
                                : connectionPath(moving, fixed));
    }

    void cancelWire()
    {
        m_wireStart = nullptr;
        delete m_temporary;
        m_temporary = nullptr;
    }

    void updateCursor()
    {
        if (m_panning)
            viewport()->setCursor(Qt::ClosedHandCursor);
        else if (m_wireStart)
            viewport()->setCursor(Qt::CrossCursor);
        else if (m_spaceDown)
            viewport()->setCursor(Qt::OpenHandCursor);
        else
            viewport()->unsetCursor();
    }

    qreal m_scale = 1.0;
    PortItem *m_wireStart = nullptr;
    QGraphicsPathItem *m_temporary = nullptr;
    bool m_spaceDown = false;
    bool m_panning = false;
    bool m_suppressDrag = false;
    bool m_cancelling = false;
    Qt::MouseButton m_panButton = Qt::NoButton;
    Qt::MouseButtons m_forwardedButtons = Qt::NoButton;
    QPoint m_lastPosition;
    QPointer<QWidget> m_watchedWindow;
};

struct GraphData
{
    explicit GraphData(QObject *owner) : scene(new NodeScene(owner)) {}

    ~GraphData()
    {
        stopCallbacks();
        // scene 是所有图元的唯一所有者；索引先失效，图元析构不访问其他图元。
        edges.clear();
        nodes.clear();
        scene.reset();
    }

    void stopCallbacks()
    {
        QObject::disconnect(selectionConnection);
        for (NodeItem *node : nodes) {
            node->clearCallbacks();
            node->incidentEdges.clear();
        }
    }

    std::unique_ptr<NodeScene> scene;
    QMap<QString, NodeItem *> nodes;
    QMap<QString, EdgeItem *> edges;
    QMetaObject::Connection selectionConnection;
};

struct NodeRecord
{
    QString id;
    NodeEditorWidget::Kind kind = NodeEditorWidget::Input;
    QString title;
    QPointF position;
};

struct EdgeRecord
{
    QString id;
    QString fromNode;
    QString fromPort;
    QString toNode;
    QString toPort;
};

QString freshId(const QString &prefix)
{
    return prefix + QUuid::createUuid().toString(QUuid::WithoutBraces);
}

bool stringField(const QJsonObject &object, const QString &field, QString *value)
{
    const QJsonValue json = object.value(field);
    if (!json.isString())
        return false;
    *value = json.toString();
    return !value->isEmpty();
}

bool validateJson(const QJsonObject &object, QMap<QString, NodeRecord> *nodes,
                  QMap<QString, EdgeRecord> *edges, QString *error)
{
    const auto fail = [error](const QString &text) {
        if (error)
            *error = text;
        return false;
    };
    const QJsonValue version = object.value(QStringLiteral("version"));
    if (!version.isDouble() || version.toDouble() != 1.0)
        return fail(QStringLiteral("version 必须是整数 1。"));
    const QJsonValue nodeValue = object.value(QStringLiteral("nodes"));
    const QJsonValue edgeValue = object.value(QStringLiteral("edges"));
    if (!nodeValue.isArray() || !edgeValue.isArray())
        return fail(QStringLiteral("nodes 和 edges 必须是数组。"));

    const QJsonArray nodeArray = nodeValue.toArray();
    int index = 0;
    for (const QJsonValue &value : nodeArray) {
        ++index;
        if (!value.isObject())
            return fail(QStringLiteral("第 %1 个节点必须是对象。").arg(index));
        const QJsonObject node = value.toObject();
        NodeRecord record;
        QString kind;
        if (!stringField(node, QStringLiteral("id"), &record.id))
            return fail(QStringLiteral("第 %1 个节点的 id 必须是非空字符串。").arg(index));
        if (nodes->contains(record.id))
            return fail(QStringLiteral("节点 id 重复：%1").arg(record.id));
        if (!stringField(node, QStringLiteral("kind"), &kind) || !parseKind(kind, &record.kind))
            return fail(QStringLiteral("节点 %1 的 kind 无效。").arg(record.id));
        if (!stringField(node, QStringLiteral("title"), &record.title))
            return fail(QStringLiteral("节点 %1 的 title 必须是非空字符串。").arg(record.id));
        const QJsonValue x = node.value(QStringLiteral("x"));
        const QJsonValue y = node.value(QStringLiteral("y"));
        if (!x.isDouble() || !y.isDouble())
            return fail(QStringLiteral("节点 %1 的 x、y 必须是有限数值。").arg(record.id));
        record.position = QPointF(x.toDouble(), y.toDouble());
        if (!validPosition(record.position))
            return fail(QStringLiteral("节点 %1 的完整范围必须位于 ±1000000 内。").arg(record.id));
        nodes->insert(record.id, record);
    }

    QMap<QString, QMap<QString, QString>> occupiedInputs;
    const QJsonArray edgeArray = edgeValue.toArray();
    index = 0;
    for (const QJsonValue &value : edgeArray) {
        ++index;
        if (!value.isObject())
            return fail(QStringLiteral("第 %1 条连线必须是对象。").arg(index));
        const QJsonObject edge = value.toObject();
        EdgeRecord record;
        if (!stringField(edge, QStringLiteral("id"), &record.id))
            return fail(QStringLiteral("第 %1 条连线的 id 必须是非空字符串。").arg(index));
        if (edges->contains(record.id))
            return fail(QStringLiteral("连线 id 重复：%1").arg(record.id));
        if (!edge.value(QStringLiteral("from")).isObject()
            || !edge.value(QStringLiteral("to")).isObject())
            return fail(QStringLiteral("连线 %1 的 from、to 必须是端点对象。").arg(record.id));
        const QJsonObject from = edge.value(QStringLiteral("from")).toObject();
        const QJsonObject to = edge.value(QStringLiteral("to")).toObject();
        if (!stringField(from, QStringLiteral("node"), &record.fromNode)
            || !stringField(from, QStringLiteral("port"), &record.fromPort)
            || !stringField(to, QStringLiteral("node"), &record.toNode)
            || !stringField(to, QStringLiteral("port"), &record.toPort))
            return fail(QStringLiteral("连线 %1 的 node、port 必须是非空字符串。").arg(record.id));
        if (!nodes->contains(record.fromNode) || !nodes->contains(record.toNode))
            return fail(QStringLiteral("连线 %1 引用了不存在的节点。").arg(record.id));
        if (record.fromNode == record.toNode)
            return fail(QStringLiteral("连线 %1 不能连接同一个节点。").arg(record.id));
        bool fromOutput = false;
        bool toOutput = false;
        if (!portDirection(nodes->value(record.fromNode).kind, record.fromPort, &fromOutput)
            || !portDirection(nodes->value(record.toNode).kind, record.toPort, &toOutput))
            return fail(QStringLiteral("连线 %1 引用了不存在的端口。").arg(record.id));
        if (!fromOutput || toOutput)
            return fail(QStringLiteral("连线 %1 必须从输出端口连接到输入端口。").arg(record.id));
        const QString previous = occupiedInputs.value(record.toNode).value(record.toPort);
        if (!previous.isEmpty()) {
            const EdgeRecord existing = edges->value(previous);
            if (existing.fromNode == record.fromNode && existing.fromPort == record.fromPort)
                return fail(QStringLiteral("连线 %1 与已有连线重复。").arg(record.id));
            return fail(QStringLiteral("节点 %1 的输入端口 %2 已被占用。")
                            .arg(record.toNode, record.toPort));
        }
        occupiedInputs[record.toNode].insert(record.toPort, record.id);
        edges->insert(record.id, record);
    }
    return true;
}

} // namespace

class NodeEditorWidget::Private
{
public:
    explicit Private(NodeEditorWidget *widget)
        : q(widget), view(new NodeView(widget)), graph(new GraphData(widget))
    {
        auto *layout = new QVBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(view);
        widget->setFocusProxy(view);
        view->setScene(graph->scene.get());
        view->centerOn(0.0, 0.0);
        view->connectionRequested = [this](PortItem *from, PortItem *to) {
            connectPorts(from, to);
        };
        view->deleteRequested = [this] { q->deleteSelected(); };
        view->selectAllRequested = [this] { q->selectAll(); };
        view->scaleChanged = [this](qreal factor) { emit q->zoomChanged(factor); };
        bindGraph(graph.get());
    }

    ~Private()
    {
        view->cancelInteraction(false);
        view->connectionRequested = nullptr;
        view->deleteRequested = nullptr;
        view->selectAllRequested = nullptr;
        view->scaleChanged = nullptr;
        graph->stopCallbacks();
        view->setScene(nullptr);
        graph.reset();
    }

    void bindNode(GraphData *state, NodeItem *node)
    {
        node->positionChanged = [this, state](NodeItem *moved) {
            for (EdgeItem *edge : moved->incidentEdges)
                edge->updateEndpoints();
            if (graph.get() == state)
                emit q->graphChanged();
        };
        node->renameRequested = [this, state](const QString &id, const QString &oldTitle) {
            if (graph.get() == state)
                requestRename(id, oldTitle);
        };
    }

    void bindGraph(GraphData *state)
    {
        for (NodeItem *node : state->nodes)
            bindNode(state, node);
        state->selectionConnection = QObject::connect(state->scene.get(),
            &QGraphicsScene::selectionChanged, q, [this, state] {
                if (graph.get() == state)
                    view->viewport()->update();
            });
    }

    void requestRename(const QString &id, const QString &oldTitle)
    {
        const quint64 generation = graphGeneration;
        QPointer<NodeEditorWidget> guard(q);
        QTimer::singleShot(0, q, [guard, id, oldTitle, generation] {
            if (!guard)
                return;
            Private *self = guard->d.get();
            if (self->graphGeneration != generation || self->renameDialogOpen)
                return;
            NodeItem *node = self->graph->nodes.value(id, nullptr);
            if (!node || node->title() != oldTitle)
                return;
            self->renameDialogOpen = true;
            bool accepted = false;
            const QString title = QInputDialog::getText(guard.data(), QStringLiteral("重命名节点"),
                QStringLiteral("节点名称："), QLineEdit::Normal, oldTitle, &accepted);
            if (!guard)
                return;
            self = guard->d.get();
            self->renameDialogOpen = false;
            if (self->graphGeneration != generation || !accepted)
                return;
            // 模态事件循环可能删除节点或替换整个图，只按稳定 ID 重新定位。
            node = self->graph->nodes.value(id, nullptr);
            if (!node || node->title() != oldTitle)
                return;
            if (title.trimmed().isEmpty()) {
                emit guard->message(QStringLiteral("节点名称不能为空。"));
                return;
            }
            if (title != oldTitle) {
                node->setTitle(title);
                emit guard->graphChanged();
            }
        });
    }

    void insertEdge(GraphData *state, const QString &id, PortItem *output, PortItem *input)
    {
        auto *edge = new EdgeItem(id, output, input);
        state->scene->addItem(edge);
        state->edges.insert(id, edge);
        output->node()->incidentEdges.insert(edge);
        input->node()->incidentEdges.insert(edge);
    }

    void connectPorts(PortItem *first, PortItem *second)
    {
        if (!first || !second)
            return;
        if (first->isOutput() == second->isOutput()) {
            emit q->message(QStringLiteral("请将输出端口连接到输入端口。"));
            return;
        }
        PortItem *output = first->isOutput() ? first : second;
        PortItem *input = first->isOutput() ? second : first;
        if (output->node() == input->node()) {
            emit q->message(QStringLiteral("不能连接同一个节点的端口。"));
            return;
        }
        for (EdgeItem *edge : input->node()->incidentEdges) {
            if (edge->input() == input) {
                emit q->message(edge->output() == output
                                    ? QStringLiteral("这两个端口已经连接。")
                                    : QStringLiteral("该输入端口已有连线，请先删除原连线。"));
                return;
            }
        }
        QString id;
        do {
            id = freshId(QStringLiteral("edge-"));
        } while (graph->edges.contains(id));
        insertEdge(graph.get(), id, output, input);
        emit q->graphChanged();
    }

    void replaceGraph(std::unique_ptr<GraphData> replacement)
    {
        view->cancelInteraction(true);
        graph->stopCallbacks();
        view->setScene(nullptr);
        graph.swap(replacement);
        ++graphGeneration;
        addSerial = 0;
        view->setScene(graph->scene.get());
        replacement.reset();
    }

    NodeEditorWidget *q;
    NodeView *view;
    std::unique_ptr<GraphData> graph;
    quint64 graphGeneration = 0;
    quint64 addSerial = 0;
    bool renameDialogOpen = false;
};

NodeEditorWidget::NodeEditorWidget(QWidget *parent)
    : QWidget(parent), d(new Private(this))
{
    setMinimumSize(240, 180);
}

NodeEditorWidget::~NodeEditorWidget() = default;

QString NodeEditorWidget::addNode(Kind kind)
{
    if (!validKind(kind))
        return QString();
    QString id;
    do {
        id = freshId(QStringLiteral("node-"));
    } while (d->graph->nodes.contains(id));
    const qreal offset = qreal(d->addSerial++ % 8) * 24.0;
    const QPointF center = d->view->mapToScene(d->view->viewport()->rect().center());
    auto *node = new NodeItem(id, kind, defaultTitle(kind));
    node->setPos(boundedPosition(center - QPointF(NodeWidth / 2.0, NodeHeight / 2.0)
                                 + QPointF(offset, offset)));
    d->graph->scene->addItem(node);
    d->graph->nodes.insert(id, node);
    d->bindNode(d->graph.get(), node);
    d->graph->scene->clearSelection();
    node->setSelected(true);
    emit graphChanged();
    return id;
}

int NodeEditorWidget::nodeCount() const
{
    return d->graph->nodes.size();
}

int NodeEditorWidget::edgeCount() const
{
    return d->graph->edges.size();
}

qreal NodeEditorWidget::zoomFactor() const
{
    return d->view->zoomFactor();
}

QJsonObject NodeEditorWidget::toJson() const
{
    QJsonArray nodes;
    for (NodeItem *node : d->graph->nodes) {
        QJsonObject record;
        record.insert(QStringLiteral("id"), node->id());
        record.insert(QStringLiteral("kind"), kindName(node->kind()));
        record.insert(QStringLiteral("title"), node->title());
        record.insert(QStringLiteral("x"), node->pos().x());
        record.insert(QStringLiteral("y"), node->pos().y());
        nodes.append(record);
    }
    QJsonArray edges;
    for (EdgeItem *edge : d->graph->edges) {
        QJsonObject from;
        from.insert(QStringLiteral("node"), edge->output()->node()->id());
        from.insert(QStringLiteral("port"), edge->output()->id());
        QJsonObject to;
        to.insert(QStringLiteral("node"), edge->input()->node()->id());
        to.insert(QStringLiteral("port"), edge->input()->id());
        QJsonObject record;
        record.insert(QStringLiteral("id"), edge->id());
        record.insert(QStringLiteral("from"), from);
        record.insert(QStringLiteral("to"), to);
        edges.append(record);
    }
    QJsonObject object;
    object.insert(QStringLiteral("version"), 1);
    object.insert(QStringLiteral("nodes"), nodes);
    object.insert(QStringLiteral("edges"), edges);
    return object;
}

bool NodeEditorWidget::fromJson(const QJsonObject &object, QString *error)
{
    QMap<QString, NodeRecord> nodes;
    QMap<QString, EdgeRecord> edges;
    if (!validateJson(object, &nodes, &edges, error))
        return false;

    std::unique_ptr<GraphData> candidate(new GraphData(this));
    for (const NodeRecord &record : nodes) {
        auto *node = new NodeItem(record.id, record.kind, record.title);
        node->setPos(record.position);
        candidate->scene->addItem(node);
        candidate->nodes.insert(record.id, node);
    }
    for (const EdgeRecord &record : edges) {
        PortItem *output = candidate->nodes.value(record.fromNode)->port(record.fromPort);
        PortItem *input = candidate->nodes.value(record.toNode)->port(record.toPort);
        d->insertEdge(candidate.get(), record.id, output, input);
    }
    // 回调绑定候选图自己的图元/邻接关系，换图前不会误读当前图的索引。
    d->bindGraph(candidate.get());
    d->replaceGraph(std::move(candidate));
    if (error)
        error->clear();
    fitToGraph();
    emit graphChanged();
    return true;
}

void NodeEditorWidget::deleteSelected()
{
    QSet<NodeItem *> nodes;
    QSet<EdgeItem *> edges;
    const QList<QGraphicsItem *> selected = d->graph->scene->selectedItems();
    for (QGraphicsItem *item : selected) {
        if (item->type() == NodeItem::Type) {
            auto *node = static_cast<NodeItem *>(item);
            nodes.insert(node);
            edges.unite(node->incidentEdges);
        } else if (item->type() == EdgeItem::Type) {
            edges.insert(static_cast<EdgeItem *>(item));
        }
    }
    d->view->cancelInteraction(true);
    if (nodes.isEmpty() && edges.isEmpty())
        return;
    for (EdgeItem *edge : edges) {
        d->graph->edges.remove(edge->id());
        edge->output()->node()->incidentEdges.remove(edge);
        edge->input()->node()->incidentEdges.remove(edge);
        delete edge;
    }
    for (NodeItem *node : nodes) {
        d->graph->nodes.remove(node->id());
        node->clearCallbacks();
        delete node;
    }
    emit graphChanged();
}

void NodeEditorWidget::selectAll()
{
    for (NodeItem *node : d->graph->nodes)
        node->setSelected(true);
    for (EdgeItem *edge : d->graph->edges)
        edge->setSelected(true);
}

void NodeEditorWidget::fitToGraph()
{
    if (d->graph->nodes.isEmpty()) {
        d->view->cancelInteraction(true);
        resetZoom();
        return;
    }
    QRectF bounds;
    bool initialized = false;
    const auto include = [&bounds, &initialized](const QRectF &rect) {
        if (!initialized) {
            bounds = rect;
            initialized = true;
        } else {
            bounds = bounds.united(rect);
        }
    };
    for (NodeItem *node : d->graph->nodes)
        include(node->mapRectToScene(node->boundingRect()));
    for (EdgeItem *edge : d->graph->edges)
        include(edge->mapRectToScene(edge->boundingRect()));
    d->view->fitGraph(bounds);
}

void NodeEditorWidget::resetZoom()
{
    d->view->setZoomAt(1.0, d->view->viewport()->rect().center());
}