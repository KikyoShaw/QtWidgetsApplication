#include "ImageCropperWidget.h"

#include <QBrush>
#include <QColor>
#include <QCursor>
#include <QEvent>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QWheelEvent>
#include <array>
#include <cmath>

namespace {

QSize aspectUnit(ImageCropperWidget::AspectMode mode)
{
    switch (mode) {
    case ImageCropperWidget::Square:
    case ImageCropperWidget::Circle:
        return QSize(1, 1);
    case ImageCropperWidget::Ratio4x3:
        return QSize(4, 3);
    case ImageCropperWidget::Ratio16x9:
        return QSize(16, 9);
    default:
        return QSize();
    }
}

int roundedBounded(qreal value, int minimum, int maximum)
{
    const qreal bounded = qBound(qreal(minimum), value, qreal(maximum));
    return static_cast<int>(std::floor(bounded + 0.5));
}

// 边拖动时保留尺寸奇偶性，才能在整数像素上保持另一轴中心不动。
int constrainedUnitCount(qreal requested, int maximum, int parity = -1)
{
    if (parity < 0)
        return roundedBounded(requested, 1, maximum);

    const int minimum = parity == 0 ? 2 : 1;
    if ((maximum & 1) != parity)
        --maximum;
    const qreal bounded = qBound(qreal(minimum), requested, qreal(maximum));
    const int steps = static_cast<int>(std::floor((bounded - minimum) / 2.0 + 0.5));
    return qBound(minimum, minimum + steps * 2, maximum);
}

} // namespace

struct ImageCropperWidget::Private
{
    enum Handle { NoHandle = -1, TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left };
    enum Drag { NoDrag, Pan, Move, Resize };

    explicit Private(ImageCropperWidget *widget) : q(widget), checker(24, 24, QImage::Format_RGB32)
    {
        if (!checker.isNull()) {
            checker.fill(QColor(34, 38, 44));
            QPainter painter(&checker);
            painter.fillRect(0, 0, 12, 12, QColor(40, 44, 51));
            painter.fillRect(12, 12, 12, 12, QColor(40, 44, 51));
        }
    }

    ImageCropperWidget *q;
    QImage image;
    QImage checker;
    QRect crop;
    AspectMode mode = Free;
    qreal scale = 1.0;
    QPointF offset;
    bool followingFit = true;
    bool spaceDown = false;
    Drag drag = NoDrag;
    Handle handle = NoHandle;
    Qt::MouseButton dragButton = Qt::NoButton;
    QPointF startMouse;
    QPointF startOffset;
    QPointF lastMouse;
    QRect startCrop;

    qreal fittedScale() const
    {
        if (image.isNull())
            return 1.0;
        const qreal availableWidth = qMax(1, q->width() - 48);
        const qreal availableHeight = qMax(1, q->height() - 48);
        return qMin(availableWidth / image.width(), availableHeight / image.height());
    }

    QPointF imagePoint(const QPointF &point) const
    {
        return (point - offset) / scale;
    }

    QRectF screenCrop() const
    {
        return QRectF(offset + QPointF(crop.x() * scale, crop.y() * scale),
                      QSizeF(crop.width() * scale, crop.height() * scale));
    }

    std::array<QPointF, 8> handles() const
    {
        const QRectF rect = screenCrop();
        const qreal left = rect.x();
        const qreal top = rect.y();
        const qreal right = left + rect.width();
        const qreal bottom = top + rect.height();
        const qreal centerX = (left + right) / 2.0;
        const qreal centerY = (top + bottom) / 2.0;
        return {{QPointF(left, top), QPointF(centerX, top), QPointF(right, top),
                 QPointF(right, centerY), QPointF(right, bottom), QPointF(centerX, bottom),
                 QPointF(left, bottom), QPointF(left, centerY)}};
    }

    Handle hitHandle(const QPointF &point) const
    {
        const auto positions = handles();
        Handle nearest = NoHandle;
        qreal nearestDistance = 0.0;
        const QPointF center = screenCrop().center();
        const Handle preferred = point.x() < center.x()
            ? (point.y() < center.y() ? TopLeft : BottomLeft)
            : (point.y() < center.y() ? TopRight : BottomRight);
        for (int i = 0; i < static_cast<int>(positions.size()); ++i) {
            const QPointF delta = point - positions[i];
            if (std::abs(delta.x()) > 9.0 || std::abs(delta.y()) > 9.0)
                continue;
            const qreal distance = delta.x() * delta.x() + delta.y() * delta.y();
            // 小选框的手柄可能重叠；按屏幕距离选择，等距时优先鼠标所在方向的角。
            if (nearest == NoHandle || distance < nearestDistance
                || (distance == nearestDistance && i == preferred)) {
                nearest = static_cast<Handle>(i);
                nearestDistance = distance;
            }
        }
        return nearest;
    }

    void updateCursor()
    {
        if (image.isNull()) {
            q->setCursor(Qt::ArrowCursor);
        } else if (drag == Pan) {
            q->setCursor(Qt::ClosedHandCursor);
        } else if (spaceDown) {
            q->setCursor(Qt::OpenHandCursor);
        } else if (drag == Move) {
            q->setCursor(Qt::SizeAllCursor);
        } else {
            const Handle hovered = drag == Resize ? handle : hitHandle(lastMouse);
            switch (hovered) {
            case TopLeft:
            case BottomRight:
                q->setCursor(Qt::SizeFDiagCursor);
                break;
            case TopRight:
            case BottomLeft:
                q->setCursor(Qt::SizeBDiagCursor);
                break;
            case Top:
            case Bottom:
                q->setCursor(Qt::SizeVerCursor);
                break;
            case Left:
            case Right:
                q->setCursor(Qt::SizeHorCursor);
                break;
            default:
                q->setCursor(screenCrop().contains(lastMouse) ? Qt::SizeAllCursor : Qt::ArrowCursor);
                break;
            }
        }
    }

    void clearInteraction(bool clearSpace = true)
    {
        drag = NoDrag;
        handle = NoHandle;
        dragButton = Qt::NoButton;
        if (clearSpace)
            spaceDown = false;
        updateCursor();
    }

    void clampOffset()
    {
        if (image.isNull())
            return;
        const qreal viewWidth = qMax(1, q->width());
        const qreal viewHeight = qMax(1, q->height());
        const qreal imageWidth = image.width() * scale;
        const qreal imageHeight = image.height() * scale;
        const qreal visibleWidth = qMin(48.0, qMin(viewWidth, imageWidth) / 2.0);
        const qreal visibleHeight = qMin(48.0, qMin(viewHeight, imageHeight) / 2.0);
        offset.setX(qBound(visibleWidth - imageWidth, offset.x(), viewWidth - visibleWidth));
        offset.setY(qBound(visibleHeight - imageHeight, offset.y(), viewHeight - visibleHeight));
    }

    void fitView(bool notify = true)
    {
        const qreal previousScale = scale;
        followingFit = true;
        scale = fittedScale();
        offset = image.isNull() ? QPointF() : QPointF(
            (q->width() - image.width() * scale) / 2.0,
            (q->height() - image.height() * scale) / 2.0);
        q->update();
        if (notify && previousScale != scale)
            emit q->zoomChanged(scale);
    }

    QRect centeredRect(const QSize &size, const QPointF &center) const
    {
        return QRect(roundedBounded(center.x() - size.width() / 2.0, 0, image.width() - size.width()),
                     roundedBounded(center.y() - size.height() / 2.0, 0, image.height() - size.height()),
                     size.width(), size.height());
    }

    QRect defaultCrop() const
    {
        if (image.isNull())
            return QRect();
        QSize size;
        if (mode == Free) {
            size = QSize(qMax(1, static_cast<int>(std::floor(image.width() * 0.8))),
                         qMax(1, static_cast<int>(std::floor(image.height() * 0.8))));
        } else {
            const QSize unit = aspectUnit(mode);
            const int maximum = qMin(image.width() / unit.width(), image.height() / unit.height());
            const int count = qMax(1, static_cast<int>(std::floor(maximum * 0.8)));
            size = QSize(unit.width() * count, unit.height() * count);
        }
        return centeredRect(size, QPointF(image.width() / 2.0, image.height() / 2.0));
    }

    void setCrop(const QRect &rect)
    {
        if (crop == rect)
            return;
        crop = rect;
        q->update();
        emit q->cropChanged(crop);
    }

    QRect movedCrop(const QPointF &delta) const
    {
        return QRect(roundedBounded(startCrop.x() + delta.x(), 0, image.width() - startCrop.width()),
                     roundedBounded(startCrop.y() + delta.y(), 0, image.height() - startCrop.height()),
                     startCrop.width(), startCrop.height());
    }

    QRect resizedCrop(const QPointF &delta) const
    {
        const bool movesLeft = handle == TopLeft || handle == Left || handle == BottomLeft;
        const bool movesRight = handle == TopRight || handle == Right || handle == BottomRight;
        const bool movesTop = handle == TopLeft || handle == Top || handle == TopRight;
        const bool movesBottom = handle == BottomLeft || handle == Bottom || handle == BottomRight;
        const int left = startCrop.x();
        const int top = startCrop.y();
        // 原图坐标统一使用半开边界，不能用 QRect::right()/bottom() 作为尺寸端点。
        const int right = left + startCrop.width();
        const int bottom = top + startCrop.height();

        if (mode == Free) {
            const int newLeft = movesLeft ? roundedBounded(left + delta.x(), 0, right - 1) : left;
            const int newRight = movesRight ? roundedBounded(right + delta.x(), left + 1, image.width()) : right;
            const int newTop = movesTop ? roundedBounded(top + delta.y(), 0, bottom - 1) : top;
            const int newBottom = movesBottom ? roundedBounded(bottom + delta.y(), top + 1, image.height()) : bottom;
            return QRect(newLeft, newTop, newRight - newLeft, newBottom - newTop);
        }

        const QSize unit = aspectUnit(mode);
        const int p = unit.width();
        const int r = unit.height();
        const int initialCount = startCrop.width() / p;
        const bool horizontal = movesLeft || movesRight;
        const bool vertical = movesTop || movesBottom;
        if (horizontal && vertical) {
            const int anchorX = movesLeft ? right : left;
            const int anchorY = movesTop ? bottom : top;
            const int maximum = qMin((movesLeft ? anchorX : image.width() - anchorX) / p,
                                     (movesTop ? anchorY : image.height() - anchorY) / r);
            const qreal directionX = movesLeft ? -1.0 : 1.0;
            const qreal directionY = movesTop ? -1.0 : 1.0;
            const qreal requested = initialCount
                + (directionX * p * delta.x() + directionY * r * delta.y()) / (p * p + r * r);
            const int count = constrainedUnitCount(requested, maximum);
            const int width = p * count;
            const int height = r * count;
            return QRect(movesLeft ? anchorX - width : anchorX,
                         movesTop ? anchorY - height : anchorY, width, height);
        }

        if (horizontal) {
            const qreal centerY = top + startCrop.height() / 2.0;
            const qreal availableHeight = 2.0 * qMin(centerY, image.height() - centerY);
            const int maximum = static_cast<int>(std::floor(qMin(
                qreal(movesLeft ? right : image.width() - left) / p, availableHeight / r)));
            const qreal requested = initialCount + (movesLeft ? -delta.x() : delta.x()) / p;
            const int count = constrainedUnitCount(requested, maximum, (r & 1) ? (initialCount & 1) : -1);
            const int width = p * count;
            const int height = r * count;
            return QRect(movesLeft ? right - width : left,
                         static_cast<int>(centerY - height / 2.0), width, height);
        }

        const qreal centerX = left + startCrop.width() / 2.0;
        const qreal availableWidth = 2.0 * qMin(centerX, image.width() - centerX);
        const int maximum = static_cast<int>(std::floor(qMin(
            availableWidth / p, qreal(movesTop ? bottom : image.height() - top) / r)));
        const qreal requested = initialCount + (movesTop ? -delta.y() : delta.y()) / r;
        const int count = constrainedUnitCount(requested, maximum, (p & 1) ? (initialCount & 1) : -1);
        const int width = p * count;
        const int height = r * count;
        return QRect(static_cast<int>(centerX - width / 2.0),
                     movesTop ? bottom - height : top, width, height);
    }

    QImage renderCrop(const QSize &size) const
    {
        if (image.isNull() || crop.isEmpty() || size.width() <= 0 || size.height() <= 0)
            return QImage();
        QImage result(size, QImage::Format_ARGB32_Premultiplied);
        if (result.isNull())
            return QImage();
        result.fill(Qt::transparent);
        QPainter painter(&result);
        painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
        const QRectF target(QPointF(0.0, 0.0), QSizeF(size));
        painter.drawImage(target, image, QRectF(crop));
        if (mode == Circle) {
            // 同一反向遮罩用于预览和导出；DestinationOut 保留圆周抗锯齿及源图透明度。
            QPainterPath outside;
            outside.setFillRule(Qt::OddEvenFill);
            outside.addRect(target);
            outside.addEllipse(target);
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
            painter.fillPath(outside, Qt::black);
        }
        return result;
    }
};

ImageCropperWidget::ImageCropperWidget(QWidget *parent) : QWidget(parent), d(new Private(this))
{
    setMinimumSize(160, 160);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setAccessibleName(QStringLiteral("图片裁剪画布"));
    setToolTip(QStringLiteral("拖动选框或手柄裁剪，滚轮缩放，中键或空格加左键平移。"));
}

ImageCropperWidget::~ImageCropperWidget() = default;

void ImageCropperWidget::setImage(const QImage &image)
{
    d->clearInteraction();
    QImage normalized = image;
    if (!normalized.isNull() && normalized.devicePixelRatio() != 1.0)
        normalized.setDevicePixelRatio(1.0);
    if (d->image == normalized)
        return;

    const QRect previousCrop = d->crop;
    const AspectMode previousMode = d->mode;
    const qreal previousScale = d->scale;
    d->image = normalized;
    if (!isAspectModeAvailable(d->mode))
        d->mode = Free;
    d->crop = d->defaultCrop();
    d->fitView(false);
    d->updateCursor();
    emit imageChanged();
    if (previousMode != d->mode)
        emit aspectModeChanged(d->mode);
    if (previousCrop != d->crop)
        emit cropChanged(d->crop);
    if (previousScale != d->scale)
        emit zoomChanged(d->scale);
}

QSize ImageCropperWidget::imageSize() const
{
    return d->image.size();
}

QRect ImageCropperWidget::cropRect() const
{
    return d->crop;
}

qreal ImageCropperWidget::zoomFactor() const
{
    return d->scale;
}

ImageCropperWidget::AspectMode ImageCropperWidget::aspectMode() const
{
    return d->mode;
}

bool ImageCropperWidget::isAspectModeAvailable(AspectMode mode) const
{
    if (mode == Free)
        return true;
    const QSize unit = aspectUnit(mode);
    return !d->image.isNull() && unit.width() > 0 && unit.height() > 0
        && d->image.width() >= unit.width() && d->image.height() >= unit.height();
}

bool ImageCropperWidget::setAspectMode(AspectMode mode)
{
    if (!isAspectModeAvailable(mode))
        return false;
    if (d->mode == mode)
        return true;

    d->clearInteraction();
    const QRect previousCrop = d->crop;
    d->mode = mode;
    if (mode != Free) {
        const QSize unit = aspectUnit(mode);
        const int count = qMax(1, qMin(d->crop.width() / unit.width(), d->crop.height() / unit.height()));
        const QSize size(unit.width() * count, unit.height() * count);
        const QPointF center(d->crop.x() + d->crop.width() / 2.0,
                             d->crop.y() + d->crop.height() / 2.0);
        d->crop = d->centeredRect(size, center);
    }
    d->updateCursor();
    update();
    emit aspectModeChanged(d->mode);
    if (previousCrop != d->crop)
        emit cropChanged(d->crop);
    return true;
}

QImage ImageCropperWidget::preview(const QSize &maximumSize) const
{
    if (d->image.isNull() || d->crop.isEmpty() || maximumSize.width() <= 0 || maximumSize.height() <= 0)
        return QImage();
    const qreal factor = qMin(qreal(maximumSize.width()) / d->crop.width(),
                              qreal(maximumSize.height()) / d->crop.height());
    const QSize size(qMax(1, qMin(maximumSize.width(), static_cast<int>(std::floor(d->crop.width() * factor)))),
                     qMax(1, qMin(maximumSize.height(), static_cast<int>(std::floor(d->crop.height() * factor)))));
    return d->renderCrop(size);
}

QImage ImageCropperWidget::croppedImage() const
{
    return d->renderCrop(d->crop.size());
}

void ImageCropperWidget::fitToWindow()
{
    d->clearInteraction();
    d->fitView();
    d->updateCursor();
}

void ImageCropperWidget::reset()
{
    d->clearInteraction();
    const QRect previousCrop = d->crop;
    const qreal previousScale = d->scale;
    d->crop = d->defaultCrop();
    d->fitView(false);
    d->updateCursor();
    if (previousCrop != d->crop)
        emit cropChanged(d->crop);
    if (previousScale != d->scale)
        emit zoomChanged(d->scale);
}

void ImageCropperWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), QColor(34, 38, 44));
    if (!d->checker.isNull())
        painter.fillRect(rect(), QBrush(d->checker));
    if (d->image.isNull()) {
        painter.setPen(QColor(186, 194, 207));
        painter.drawText(rect().adjusted(16, 16, -16, -16), Qt::AlignCenter | Qt::TextWordWrap,
                         QStringLiteral("打开图片开始裁剪\n拖动选框与手柄调整裁剪区域"));
        return;
    }

    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    const QRectF imageRect(d->offset, QSizeF(d->image.width() * d->scale, d->image.height() * d->scale));
    painter.drawImage(imageRect, d->image, QRectF(d->image.rect()));
    const QRectF crop = d->screenCrop();
    QPainterPath selection;
    if (d->mode == Circle)
        selection.addEllipse(crop);
    else
        selection.addRect(crop);
    QPainterPath outside;
    outside.setFillRule(Qt::OddEvenFill);
    outside.addRect(QRectF(rect()));
    outside.addPath(selection);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(outside, QColor(0, 0, 0, 155));

    painter.save();
    painter.setClipPath(selection, Qt::IntersectClip);
    painter.setPen(QPen(QColor(255, 255, 255, 130), 1.0));
    for (int i = 1; i <= 2; ++i) {
        const qreal x = crop.x() + crop.width() * i / 3.0;
        const qreal y = crop.y() + crop.height() * i / 3.0;
        painter.drawLine(QPointF(x, crop.y()), QPointF(x, crop.y() + crop.height()));
        painter.drawLine(QPointF(crop.x(), y), QPointF(crop.x() + crop.width(), y));
    }
    painter.restore();
    painter.setPen(QPen(QColor(103, 188, 255), 1.5));
    painter.setBrush(Qt::NoBrush);
    painter.drawPath(selection);
    painter.setPen(QPen(QColor(34, 91, 139), 1.0));
    painter.setBrush(QColor(244, 249, 255));
    for (const QPointF &point : d->handles())
        painter.drawRect(QRectF(point.x() - 3.5, point.y() - 3.5, 7.0, 7.0));

    if (d->mode == Circle) {
        const QRect label(12, 12, qMin(width() - 24, 188), 28);
        painter.fillRect(label, QColor(20, 25, 32, 215));
        painter.setPen(QColor(229, 238, 249));
        painter.drawText(label, Qt::AlignCenter, QStringLiteral("圆形输出 · 四角透明"));
    }
}

void ImageCropperWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    d->clearInteraction(false);
    if (d->followingFit || event->oldSize().width() <= 0 || event->oldSize().height() <= 0) {
        d->fitView();
    } else if (!d->image.isNull()) {
        const QPointF previousCenter(event->oldSize().width() / 2.0, event->oldSize().height() / 2.0);
        const QPointF sourceCenter = d->imagePoint(previousCenter);
        d->offset = QPointF(width() / 2.0, height() / 2.0) - sourceCenter * d->scale;
        d->clampOffset();
        update();
    }
    d->updateCursor();
}

void ImageCropperWidget::mousePressEvent(QMouseEvent *event)
{
    if (d->image.isNull()) {
        QWidget::mousePressEvent(event);
        return;
    }
    d->lastMouse = event->localPos();
    if (event->button() == Qt::MiddleButton || (event->button() == Qt::LeftButton && d->spaceDown)) {
        d->drag = Private::Pan;
    } else if (event->button() == Qt::LeftButton) {
        d->handle = d->hitHandle(d->lastMouse);
        if (d->handle != Private::NoHandle)
            d->drag = Private::Resize;
        else if (d->screenCrop().contains(d->lastMouse))
            d->drag = Private::Move;
        else {
            QWidget::mousePressEvent(event);
            return;
        }
    } else {
        QWidget::mousePressEvent(event);
        return;
    }
    setFocus(Qt::MouseFocusReason);
    d->dragButton = event->button();
    d->startMouse = d->lastMouse;
    d->startOffset = d->offset;
    d->startCrop = d->crop;
    d->updateCursor();
    event->accept();
}

void ImageCropperWidget::mouseMoveEvent(QMouseEvent *event)
{
    d->lastMouse = event->localPos();
    if (d->drag != Private::NoDrag && !(event->buttons() & d->dragButton))
        d->clearInteraction(false);
    if (d->drag == Private::Pan) {
        d->followingFit = false;
        d->offset = d->startOffset + d->lastMouse - d->startMouse;
        d->clampOffset();
        update();
    } else if (d->drag == Private::Move || d->drag == Private::Resize) {
        const QPointF delta = (d->lastMouse - d->startMouse) / d->scale;
        d->setCrop(d->drag == Private::Move ? d->movedCrop(delta) : d->resizedCrop(delta));
    }
    d->updateCursor();
    event->accept();
}

void ImageCropperWidget::mouseReleaseEvent(QMouseEvent *event)
{
    d->lastMouse = event->localPos();
    if (event->button() == d->dragButton) {
        d->clearInteraction(false);
        event->accept();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void ImageCropperWidget::wheelEvent(QWheelEvent *event)
{
    if (d->image.isNull()) {
        QWidget::wheelEvent(event);
        return;
    }
    event->accept();
    if (d->drag != Private::NoDrag)
        return;
    const QPoint pixels = event->pixelDelta();
    const qreal steps = !pixels.isNull() ? pixels.y() / 120.0 : event->angleDelta().y() / 120.0;
    if (steps == 0.0)
        return;
    const QPointF mouse = event->posF();
    const QPointF source = d->imagePoint(mouse);
    const qreal fitted = d->fittedScale();
    // 浮点步进累积细粒度滚轮输入；下限跟随 fit，超大原图仍然能够完整显示。
    const qreal scale = qBound(fitted / 16.0, d->scale * std::pow(1.2, qBound(-100.0, steps, 100.0)),
                               qMax(32.0, fitted));
    if (scale == d->scale)
        return;
    d->followingFit = false;
    d->scale = scale;
    d->offset = mouse - source * scale;
    d->clampOffset();
    d->lastMouse = mouse;
    d->updateCursor();
    update();
    emit zoomChanged(scale);
}

void ImageCropperWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        if (!event->isAutoRepeat()) {
            d->clearInteraction(false);
            d->spaceDown = true;
            d->updateCursor();
        }
        event->accept();
        return;
    }
    QWidget::keyPressEvent(event);
}

void ImageCropperWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space) {
        if (!event->isAutoRepeat())
            d->clearInteraction();
        event->accept();
        return;
    }
    QWidget::keyReleaseEvent(event);
}

void ImageCropperWidget::focusOutEvent(QFocusEvent *event)
{
    d->clearInteraction();
    QWidget::focusOutEvent(event);
}

bool ImageCropperWidget::event(QEvent *event)
{
    if (d && (event->type() == QEvent::WindowDeactivate || event->type() == QEvent::Hide))
        d->clearInteraction();
    else if (d && event->type() == QEvent::UngrabMouse)
        d->clearInteraction(false);
    return QWidget::event(event);
}
