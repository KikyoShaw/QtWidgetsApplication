#pragma once

#include <QImage>
#include <QRect>
#include <QPoint>
#include <QPointF>
#include <QString>
#include <QColor>
#include <QFont>
#include <QVector>

namespace ScreenCaptureAnnotations {

enum class Kind { Mosaic, Text, Arrow };

struct Annotation {
    Kind kind = Kind::Arrow;
    QRect clip;       // Creation-time selection in physical pixels.
    QRect rectangle;  // Half-open mosaic region or text layout box.
    QPointF start;
    QPointF end;
    QString text;
    QColor color = Qt::red;
    QFont font;       // Text pixelSize is measured in physical pixels.
    qreal lineWidth = 3.0;
    int mosaicSize = 12;
};

bool isValid(const Annotation &annotation);
void apply(QImage &canvas, const Annotation &annotation);
QImage render(const QImage &base, const QVector<Annotation> &annotations);
QImage preview(const QImage &composited, const Annotation &annotation,
               QPoint *position);

} // namespace ScreenCaptureAnnotations
