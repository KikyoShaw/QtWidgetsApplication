#include "ScreenCaptureAnnotation.h"

#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QPolygonF>
#include <QRectF>
#include <QTextDocument>
#include <QTextOption>

#include <algorithm>
#include <cmath>

namespace ScreenCaptureAnnotations {
namespace {

struct ArrowGeometry {
    QPolygonF head;
};

bool finitePoint(const QPointF &point)
{
    return std::isfinite(point.x()) && std::isfinite(point.y());
}

bool makeArrowGeometry(const Annotation &annotation, ArrowGeometry &geometry)
{
    if (!finitePoint(annotation.start) || !finitePoint(annotation.end)
        || !std::isfinite(annotation.lineWidth) || annotation.lineWidth <= 0.0) {
        return false;
    }

    const qreal dx = annotation.end.x() - annotation.start.x();
    const qreal dy = annotation.end.y() - annotation.start.y();
    const qreal length = std::hypot(dx, dy);
    if (!std::isfinite(length) || length <= 0.0) {
        return false;
    }

    const QPointF direction(dx / length, dy / length);
    const QPointF normal(-direction.y(), direction.x());
    const qreal headLength = std::min(length * 0.35,
                                    std::max(qreal(10.0), annotation.lineWidth * 4.0));
    const QPointF headBase = annotation.end - direction * headLength;
    const QPointF left = headBase + normal * (headLength * 0.45);
    const QPointF right = headBase - normal * (headLength * 0.45);
    if (!finitePoint(left) || !finitePoint(right)) {
        return false;
    }

    geometry.head.clear();
    geometry.head.append(annotation.end);
    geometry.head.append(left);
    geometry.head.append(right);
    return true;
}

QRect affectedRectangle(const QImage &canvas, const Annotation &annotation)
{
    const QRect clip = annotation.clip.intersected(canvas.rect());
    if (clip.isEmpty()) {
        return QRect();
    }
    if (annotation.kind != Kind::Arrow) {
        return annotation.rectangle.intersected(clip);
    }

    ArrowGeometry geometry;
    if (!makeArrowGeometry(annotation, geometry)) {
        return QRect();
    }

    qreal left = std::min(annotation.start.x(), annotation.end.x());
    qreal top = std::min(annotation.start.y(), annotation.end.y());
    qreal right = std::max(annotation.start.x(), annotation.end.x());
    qreal bottom = std::max(annotation.start.y(), annotation.end.y());
    for (const QPointF &point : geometry.head) {
        left = std::min(left, point.x());
        top = std::min(top, point.y());
        right = std::max(right, point.x());
        bottom = std::max(bottom, point.y());
    }

    // Include round caps and antialiasing, then clamp before converting to int.
    const qreal padding = annotation.lineWidth * 0.5 + 2.0;
    left = std::max(qreal(clip.x()), left - padding);
    top = std::max(qreal(clip.y()), top - padding);
    right = std::min(qreal(clip.x()) + clip.width(), right + padding);
    bottom = std::min(qreal(clip.y()) + clip.height(), bottom + padding);
    if (right <= left || bottom <= top) {
        return QRect();
    }

    const int x = static_cast<int>(std::floor(left));
    const int y = static_cast<int>(std::floor(top));
    const int endX = static_cast<int>(std::ceil(right));
    const int endY = static_cast<int>(std::ceil(bottom));
    return QRect(x, y, endX - x, endY - y);
}

bool pixelate(QImage &canvas, const QRect &rectangle, int blockSize)
{
    const int columns = 1 + (rectangle.width() - 1) / blockSize;
    const int rows = 1 + (rectangle.height() - 1) / blockSize;
    QImage samples(columns, rows, QImage::Format_ARGB32_Premultiplied);
    if (samples.isNull()) {
        return false;
    }
    samples.fill(Qt::transparent);

    // Finish reading the current composite before opening a painter on canvas.
    {
        QPainter sampler(&samples);
        if (!sampler.isActive()) {
            return false;
        }
        sampler.setCompositionMode(QPainter::CompositionMode_Source);
        sampler.setRenderHint(QPainter::SmoothPixmapTransform, true);
        sampler.drawImage(QRectF(samples.rect()), canvas, QRectF(rectangle));
    }

    QPainter painter(&canvas);
    if (!painter.isActive()) {
        return false;
    }
    painter.setClipRect(rectangle);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
    painter.drawImage(QRectF(rectangle), samples, QRectF(samples.rect()));
    return true;
}

void drawArrow(QPainter &painter, const Annotation &annotation,
               const ArrowGeometry &geometry)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(annotation.color, annotation.lineWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(annotation.start, annotation.end);
    painter.setPen(Qt::NoPen);
    painter.setBrush(annotation.color);
    painter.drawPolygon(geometry.head);
}

void drawText(QPainter &painter, QImage &canvas, const Annotation &annotation,
              const QRect &clip)
{
    QTextDocument document;
    document.setDocumentMargin(0.0);
    document.setDefaultFont(annotation.font);
    QTextOption option = document.defaultTextOption();
    option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    document.setDefaultTextOption(option);
    document.setPlainText(annotation.text);
    document.documentLayout()->setPaintDevice(&canvas);
    document.setTextWidth(annotation.rectangle.width());

    QAbstractTextDocumentLayout::PaintContext context;
    context.palette.setColor(QPalette::Text, annotation.color);
    context.clip = QRectF(clip.intersected(annotation.rectangle));
    context.clip.translate(-qreal(annotation.rectangle.x()),
                           -qreal(annotation.rectangle.y()));

    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setClipRect(annotation.rectangle, Qt::IntersectClip);
    painter.translate(qreal(annotation.rectangle.x()),
                      qreal(annotation.rectangle.y()));
    document.documentLayout()->draw(&painter, context);
}

// The public mutator and both image-producing paths share this implementation.
// A false result lets render/preview propagate image allocation/paint failures.
bool applyAnnotation(QImage &canvas, const Annotation &annotation)
{
    if (canvas.isNull()) {
        return false;
    }
    canvas.setDevicePixelRatio(1.0);
    if (!isValid(annotation)) {
        return true;
    }

    const QRect clip = annotation.clip.intersected(canvas.rect());
    if (clip.isEmpty()) {
        return true;
    }
    if (annotation.kind == Kind::Mosaic) {
        const QRect rectangle = annotation.rectangle.intersected(clip);
        return rectangle.isEmpty() || pixelate(canvas, rectangle, annotation.mosaicSize);
    }
    if (annotation.kind == Kind::Text
        && annotation.rectangle.intersected(clip).isEmpty()) {
        return true;
    }

    ArrowGeometry geometry;
    if (annotation.kind == Kind::Arrow && !makeArrowGeometry(annotation, geometry)) {
        return true;
    }

    QPainter painter(&canvas);
    if (!painter.isActive()) {
        return false;
    }
    painter.setClipRect(clip);
    if (annotation.kind == Kind::Arrow) {
        drawArrow(painter, annotation, geometry);
    } else {
        drawText(painter, canvas, annotation, clip);
    }
    return true;
}

} // namespace

bool isValid(const Annotation &annotation)
{
    if (annotation.clip.isEmpty()) {
        return false;
    }

    switch (annotation.kind) {
    case Kind::Mosaic:
        return !annotation.rectangle.isEmpty() && annotation.mosaicSize > 0;
    case Kind::Text:
        return !annotation.rectangle.isEmpty() && !annotation.text.trimmed().isEmpty()
            && annotation.font.pixelSize() > 0 && annotation.color.isValid();
    case Kind::Arrow: {
        ArrowGeometry geometry;
        return annotation.color.isValid() && makeArrowGeometry(annotation, geometry);
    }
    }
    return false;
}

void apply(QImage &canvas, const Annotation &annotation)
{
    applyAnnotation(canvas, annotation);
}

QImage render(const QImage &base, const QVector<Annotation> &annotations)
{
    if (base.isNull()) {
        return QImage();
    }

    QImage result = annotations.isEmpty() ? base : base.copy();
    if (result.isNull()) {
        return QImage();
    }
    result.setDevicePixelRatio(1.0);
    for (const Annotation &annotation : annotations) {
        if (!applyAnnotation(result, annotation)) {
            return QImage();
        }
    }
    return result;
}

QImage preview(const QImage &composited, const Annotation &annotation, QPoint *position)
{
    if (position) {
        *position = QPoint();
    }
    if (composited.isNull() || !isValid(annotation)) {
        return QImage();
    }

    const QRect rectangle = affectedRectangle(composited, annotation);
    if (rectangle.isEmpty()) {
        return QImage();
    }
    QImage patch = composited.copy(rectangle);
    if (patch.isNull()) {
        return QImage();
    }
    patch.setDevicePixelRatio(1.0);

    const QPoint offset(-rectangle.x(), -rectangle.y());
    Annotation shifted = annotation;
    shifted.clip = annotation.clip.intersected(composited.rect()).translated(offset);
    shifted.rectangle.translate(offset);
    shifted.start += QPointF(offset);
    shifted.end += QPointF(offset);

    // For mosaic, rectangle is exactly the full apply ROI. Keeping this origin
    // preserves both the sample count and grid alignment in the local patch.
    if (!applyAnnotation(patch, shifted)) {
        return QImage();
    }
    if (position) {
        *position = rectangle.topLeft();
    }
    return patch;
}

} // namespace ScreenCaptureAnnotations
