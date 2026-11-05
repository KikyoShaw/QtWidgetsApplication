#include "ScreenCaptureOverlay.h"

#include <QCloseEvent>
#include <QFontMetrics>
#include <QGridLayout>
#include <QKeySequence>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <QPushButton>
#include <QResizeEvent>
#include <QShortcut>
#include <QSizePolicy>
#include <QStyle>
#include <QStyleOptionButton>
#include <cmath>

ScreenCaptureOverlay::ScreenCaptureOverlay(const QImage &frozenImage, QWidget *parent)
    : QWidget(parent, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint),
      image(frozenImage)
{
    image.setDevicePixelRatio(1.0);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);

    toolbar = new QWidget(this);
    toolbar->setAttribute(Qt::WA_StyledBackground);
    toolbar->setStyleSheet(QStringLiteral(
        "QWidget { background: #252b34; color: white; border-radius: 5px; }"
        "QPushButton { padding: 5px 10px; background: #394250; border: 1px solid #536071; }"
        "QPushButton:hover { background: #4a586b; }"
        "QPushButton:pressed { background: #263442; }"
        "QPushButton:focus { border-color: #71c8ff; }"));
    toolbarLayout = new QGridLayout(toolbar);
    toolbarLayout->setContentsMargins(8, 8, 8, 8);
    toolbarLayout->setSpacing(6);
    toolbarLayout->setSizeConstraint(QLayout::SetNoConstraint);

    const std::array<QString, 4> texts = {{QStringLiteral("重选"), QStringLiteral("复制"),
                                         QStringLiteral("保存 PNG"), QStringLiteral("取消")}};
    for (int i = 0; i < static_cast<int>(buttons.size()); ++i) {
        buttons[i] = new QPushButton(texts[i], toolbar);
        buttons[i]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        buttons[i]->setMinimumSize(0, 28);
        toolbarLayout->addWidget(buttons[i], 0, i);
    }
    connect(buttons[0], &QPushButton::clicked, this, [this]() { emit reselectRequested(); });
    connect(buttons[1], &QPushButton::clicked, this,
            [this]() { requestCapture(ScreenCaptureController::Copy); });
    connect(buttons[2], &QPushButton::clicked, this,
            [this]() { requestCapture(ScreenCaptureController::Save); });
    connect(buttons[3], &QPushButton::clicked, this, [this]() { emit cancelRequested(); });
    toolbar->hide();

    auto *copyShortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    copyShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(copyShortcut, &QShortcut::activated, this,
            [this]() { requestCapture(ScreenCaptureController::Copy); });
    auto *keypadCopyShortcut = new QShortcut(QKeySequence(Qt::Key_Enter), this);
    keypadCopyShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(keypadCopyShortcut, &QShortcut::activated, this,
            [this]() { requestCapture(ScreenCaptureController::Copy); });
    auto *saveShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    saveShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(saveShortcut, &QShortcut::activated, this,
            [this]() { requestCapture(ScreenCaptureController::Save); });
    auto *cancelShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    cancelShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(cancelShortcut, &QShortcut::activated, this, [this]() { emit cancelRequested(); });
}

QRect ScreenCaptureOverlay::physicalRect() const
{
    if (image.isNull() || image.width() <= 0 || image.height() <= 0
        || width() <= 0 || height() <= 0 || selection.isEmpty()) {
        return QRect();
    }

    // 逻辑边界是半开区间；只在此处用实际截图尺寸映射到物理像素。
    const qreal ratioX = qreal(image.width()) / width();
    const qreal ratioY = qreal(image.height()) / height();
    const int left = qBound(0, static_cast<int>(std::floor(selection.left() * ratioX)), image.width() - 1);
    const int top = qBound(0, static_cast<int>(std::floor(selection.top() * ratioY)), image.height() - 1);
    const int right = qBound(left + 1, static_cast<int>(std::ceil(selection.right() * ratioX)), image.width());
    const int bottom = qBound(top + 1, static_cast<int>(std::ceil(selection.bottom() * ratioY)), image.height());
    return QRect(left, top, right - left, bottom - top);
}

QImage ScreenCaptureOverlay::resultImage() const
{
    const QRect pixels = physicalRect();
    if (pixels.isEmpty())
        return QImage();
    QImage result = image.copy(pixels);
    result.setDevicePixelRatio(1.0);
    return result;
}

void ScreenCaptureOverlay::resetSelection()
{
    drag = NoDrag;
    handle = NoHandle;
    selection = QRectF();
    initialSelection = QRectF();
    toolbar->hide();
    setCursor(Qt::CrossCursor);
    update();
}

QPointF ScreenCaptureOverlay::boundedPoint(const QPointF &point) const
{
    return QPointF(qBound(qreal(0), point.x(), qreal(qMax(0, width()))),
                   qBound(qreal(0), point.y(), qreal(qMax(0, height()))));
}

QSizeF ScreenCaptureOverlay::minimumSelectionSize() const
{
    if (image.width() <= 0 || image.height() <= 0 || width() <= 0 || height() <= 0)
        return QSizeF();
    return QSizeF(qreal(width()) / image.width(), qreal(height()) / image.height());
}

QRectF ScreenCaptureOverlay::newSelection(const QPointF &anchor, const QPointF &point) const
{
    const QSizeF minimum = minimumSelectionSize();
    if (minimum.isEmpty())
        return QRectF();

    qreal left = qMin(anchor.x(), point.x());
    qreal right = qMax(anchor.x(), point.x());
    qreal top = qMin(anchor.y(), point.y());
    qreal bottom = qMax(anchor.y(), point.y());
    if (right - left < minimum.width()) {
        if (point.x() < anchor.x()) {
            left = qMax(qreal(0), right - minimum.width());
            right = qMin(qreal(width()), left + minimum.width());
        } else {
            right = qMin(qreal(width()), left + minimum.width());
            left = qMax(qreal(0), right - minimum.width());
        }
    }
    if (bottom - top < minimum.height()) {
        if (point.y() < anchor.y()) {
            top = qMax(qreal(0), bottom - minimum.height());
            bottom = qMin(qreal(height()), top + minimum.height());
        } else {
            bottom = qMin(qreal(height()), top + minimum.height());
            top = qMax(qreal(0), bottom - minimum.height());
        }
    }
    return QRectF(left, top, right - left, bottom - top);
}

std::array<QPointF, 8> ScreenCaptureOverlay::handlePositions() const
{
    const qreal left = selection.left();
    const qreal right = selection.right();
    const qreal top = selection.top();
    const qreal bottom = selection.bottom();
    const qreal centerX = (left + right) / 2.0;
    const qreal centerY = (top + bottom) / 2.0;
    return {{QPointF(left, top), QPointF(centerX, top), QPointF(right, top),
             QPointF(right, centerY), QPointF(right, bottom), QPointF(centerX, bottom),
             QPointF(left, bottom), QPointF(left, centerY)}};
}

ScreenCaptureOverlay::Handle ScreenCaptureOverlay::hitHandle(const QPointF &point) const
{
    if (selection.isEmpty())
        return NoHandle;
    const auto positions = handlePositions();
    Handle nearest = NoHandle;
    qreal nearestDistance = 0.0;
    const Handle preferred = point.x() < selection.center().x()
        ? (point.y() < selection.center().y() ? TopLeft : BottomLeft)
        : (point.y() < selection.center().y() ? TopRight : BottomRight);
    for (int i = 0; i < static_cast<int>(positions.size()); ++i) {
        const QPointF delta = point - positions[i];
        if (std::abs(delta.x()) > 9.0 || std::abs(delta.y()) > 9.0)
            continue;
        const qreal distance = delta.x() * delta.x() + delta.y() * delta.y();
        if (nearest == NoHandle || distance < nearestDistance
            || (distance == nearestDistance && i == preferred)) {
            nearest = static_cast<Handle>(i);
            nearestDistance = distance;
        }
    }
    return nearest;
}

bool ScreenCaptureOverlay::containsSelection(const QPointF &point) const
{
    return !selection.isEmpty() && point.x() >= selection.left() && point.x() < selection.right()
        && point.y() >= selection.top() && point.y() < selection.bottom();
}

void ScreenCaptureOverlay::updateSelection(const QPointF &mousePoint)
{
    const QPointF point = boundedPoint(mousePoint);
    const QPointF delta = point - pressPoint;
    if (drag == CreateSelection) {
        selection = newSelection(pressPoint, point);
    } else if (drag == MoveSelection) {
        const qreal dx = qBound(-initialSelection.left(), delta.x(), qreal(width()) - initialSelection.right());
        const qreal dy = qBound(-initialSelection.top(), delta.y(), qreal(height()) - initialSelection.bottom());
        selection = initialSelection.translated(dx, dy);
    } else if (drag == ResizeSelection) {
        const QSizeF minimum = minimumSelectionSize();
        qreal left = initialSelection.left();
        qreal right = initialSelection.right();
        qreal top = initialSelection.top();
        qreal bottom = initialSelection.bottom();
        if (handle == TopLeft || handle == Left || handle == BottomLeft)
            left = qBound(qreal(0), initialSelection.left() + delta.x(), qMax(qreal(0), right - minimum.width()));
        if (handle == TopRight || handle == Right || handle == BottomRight)
            right = qBound(qMin(qreal(width()), left + minimum.width()), initialSelection.right() + delta.x(), qreal(width()));
        if (handle == TopLeft || handle == Top || handle == TopRight)
            top = qBound(qreal(0), initialSelection.top() + delta.y(), qMax(qreal(0), bottom - minimum.height()));
        if (handle == BottomLeft || handle == Bottom || handle == BottomRight)
            bottom = qBound(qMin(qreal(height()), top + minimum.height()), initialSelection.bottom() + delta.y(), qreal(height()));
        selection = QRectF(left, top, right - left, bottom - top);
    }
    update();
}

void ScreenCaptureOverlay::updateCursor(const QPointF &point)
{
    const Handle cursorHandle = drag == ResizeSelection ? handle : hitHandle(point);
    switch (cursorHandle) {
    case TopLeft:
    case BottomRight:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TopRight:
    case BottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case Top:
    case Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case Left:
    case Right:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        setCursor(drag == MoveSelection || containsSelection(point) ? Qt::SizeAllCursor : Qt::CrossCursor);
        break;
    }
}

void ScreenCaptureOverlay::arrangeToolbar()
{
    if (selection.isEmpty() || width() <= 0 || height() <= 0)
        return;

    const int margin = qMin(8, qMin(width() / 2, height() / 2));
    const int availableWidth = qMax(1, width() - margin * 2);
    const int availableHeight = qMax(1, height() - margin * 2);
    toolbar->ensurePolished();
    const QMargins contentsMargins = toolbarLayout->contentsMargins();
    const int horizontalMargins = contentsMargins.left() + contentsMargins.right();
    const int verticalMargins = contentsMargins.top() + contentsMargins.bottom();
    const int horizontalSpacing = toolbarLayout->horizontalSpacing();
    const int verticalSpacing = toolbarLayout->verticalSpacing();
    const int buttonCount = static_cast<int>(buttons.size());
    int widest = 0;
    int tallest = 28;
    for (QPushButton *button : buttons) {
        button->ensurePolished();
        QStyleOptionButton option;
        option.initFrom(button);
        option.text = button->text();
        const QSize textSize = button->fontMetrics().size(
            Qt::TextShowMnemonic | Qt::TextSingleLine, button->text());
        const QSize styledSize = button->style()->sizeFromContents(
            QStyle::CT_PushButton, &option, textSize, button);
        const QSize hint = button->sizeHint().expandedTo(button->minimumSizeHint())
                               .expandedTo(styledSize);
        widest = qMax(widest, hint.width());
        tallest = qMax(tallest, hint.height());
        toolbarLayout->removeWidget(button);
    }

    // 换列前清空旧约束，避免 QGridLayout 为已空出的列保留宽度。
    for (int column = 0; column < toolbarLayout->columnCount(); ++column) {
        toolbarLayout->setColumnMinimumWidth(column, 0);
        toolbarLayout->setColumnStretch(column, 0);
    }
    // 按最宽按钮统一列宽，换行判断和工具条宽度使用相同尺寸。
    const int fullWidth = horizontalMargins + widest * buttonCount
        + horizontalSpacing * (buttonCount - 1);
    const int twoColumnWidth = horizontalMargins + widest * 2 + horizontalSpacing;
    const int columns = fullWidth <= availableWidth ? 4 : (twoColumnWidth <= availableWidth ? 2 : 1);
    const int rows = (buttonCount + columns - 1) / columns;
    for (int column = 0; column < columns; ++column) {
        toolbarLayout->setColumnMinimumWidth(column, widest);
        toolbarLayout->setColumnStretch(column, 1);
    }
    for (int i = 0; i < buttonCount; ++i) {
        buttons[i]->setMinimumSize(widest, tallest);
        toolbarLayout->addWidget(buttons[i], i / columns, i % columns);
    }

    const int naturalWidth = horizontalMargins + widest * columns
        + horizontalSpacing * (columns - 1);
    const int toolbarWidth = qMin(availableWidth, naturalWidth);
    const int toolbarHeight = qMin(availableHeight,
        verticalMargins + tallest * rows + verticalSpacing * (rows - 1));
    qreal x = selection.right() - toolbarWidth;
    qreal y = selection.bottom() + 12;
    if (y + toolbarHeight > height() - margin)
        y = selection.top() - toolbarHeight - 12;
    if (y < margin)
        y = selection.bottom() - toolbarHeight - 12;
    x = qBound(qreal(margin), x, qreal(width() - margin - toolbarWidth));
    y = qBound(qreal(margin), y, qreal(height() - margin - toolbarHeight));
    toolbar->setGeometry(static_cast<int>(std::floor(x)), static_cast<int>(std::floor(y)),
                         toolbarWidth, toolbarHeight);
    toolbarLayout->activate();
}

void ScreenCaptureOverlay::requestCapture(ScreenCaptureController::Action action)
{
    if (isVisible() && drag == NoDrag && !selection.isEmpty())
        emit captureRequested(action);
}

void ScreenCaptureOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // 原图始终保留完整物理像素；缩放只用于屏幕显示，不参与最终裁剪。
    painter.drawImage(QRectF(rect()), image, QRectF(image.rect()));
    QPainterPath shade;
    shade.setFillRule(Qt::OddEvenFill);
    shade.addRect(QRectF(rect()));
    if (!selection.isEmpty())
        shade.addRect(selection);
    painter.fillPath(shade, QColor(0, 0, 0, 145));

    if (selection.isEmpty()) {
        const QString hint = QStringLiteral("拖动选择区域");
        const QSize textSize = painter.fontMetrics().size(Qt::TextSingleLine, hint) + QSize(28, 18);
        const QSize boxSize(qMin(textSize.width(), width()), qMin(textSize.height(), height()));
        const QRect box(QPoint((width() - boxSize.width()) / 2, (height() - boxSize.height()) / 2), boxSize);
        painter.fillRect(box, QColor(25, 31, 40, 225));
        painter.setPen(Qt::white);
        painter.drawText(box, Qt::AlignCenter, hint);
        return;
    }

    painter.setPen(QPen(QColor(86, 190, 255), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(selection);
    painter.setBrush(Qt::white);
    for (const QPointF &point : handlePositions())
        painter.drawRect(QRectF(point.x() - 3, point.y() - 3, 6, 6));

    const QRect pixels = physicalRect();
    const QString sizeText = QStringLiteral("%1 × %2 像素").arg(pixels.width()).arg(pixels.height());
    QSize labelSize = painter.fontMetrics().size(Qt::TextSingleLine, sizeText) + QSize(16, 10);
    labelSize.setWidth(qMin(labelSize.width(), width()));
    labelSize.setHeight(qMin(labelSize.height(), height()));
    qreal labelY = selection.top() - labelSize.height() - 8;
    if (labelY < 0)
        labelY = selection.top() + 10;
    const int x = static_cast<int>(qBound(qreal(0), selection.left(), qreal(width() - labelSize.width())));
    const int y = static_cast<int>(qBound(qreal(0), labelY, qreal(height() - labelSize.height())));
    const QRect label(QPoint(x, y), labelSize);
    painter.fillRect(label, QColor(25, 31, 40, 235));
    painter.setPen(Qt::white);
    painter.drawText(label, Qt::AlignCenter, sizeText);
}

void ScreenCaptureOverlay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (!selection.isEmpty()) {
        selection = selection.intersected(QRectF(rect()));
        if (!selection.isEmpty())
            selection = newSelection(selection.topLeft(), selection.bottomRight());
    }
    if (selection.isEmpty())
        toolbar->hide();
    else if (toolbar->isVisible())
        arrangeToolbar();
    update();
}

void ScreenCaptureOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !isVisible() || image.isNull()) {
        event->ignore();
        return;
    }
    if (toolbar->isVisible() && toolbar->geometry().contains(event->localPos().toPoint())) {
        event->accept();
        return;
    }

    QPointer<ScreenCaptureOverlay> guard(this);
    emit selectionStarted();
    if (!guard || !isVisible())
        return;
    setFocus(Qt::MouseFocusReason);
    pressPoint = boundedPoint(event->localPos());
    initialSelection = selection;
    handle = hitHandle(pressPoint);
    if (handle != NoHandle) {
        drag = ResizeSelection;
    } else if (containsSelection(pressPoint)) {
        drag = MoveSelection;
    } else {
        drag = CreateSelection;
        selection = newSelection(pressPoint, pressPoint);
    }
    toolbar->hide();
    updateCursor(pressPoint);
    update();
    event->accept();
}

void ScreenCaptureOverlay::mouseMoveEvent(QMouseEvent *event)
{
    if (drag != NoDrag)
        updateSelection(event->localPos());
    updateCursor(boundedPoint(event->localPos()));
    event->accept();
}

void ScreenCaptureOverlay::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || drag == NoDrag) {
        event->ignore();
        return;
    }
    updateSelection(event->localPos());
    drag = NoDrag;
    handle = NoHandle;
    if (!selection.isEmpty()) {
        arrangeToolbar();
        toolbar->show();
        toolbar->raise();
    }
    updateCursor(boundedPoint(event->localPos()));
    event->accept();
}

void ScreenCaptureOverlay::closeEvent(QCloseEvent *event)
{
    event->ignore();
    emit cancelRequested();
}