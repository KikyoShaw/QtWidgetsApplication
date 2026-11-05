#pragma once

#include "ScreenCaptureController.h"

#include <QImage>
#include <QPointF>
#include <QRectF>
#include <QWidget>
#include <array>

class QCloseEvent;
class QGridLayout;
class QMouseEvent;
class QPaintEvent;
class QPushButton;
class QResizeEvent;

class ScreenCaptureOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenCaptureOverlay(const QImage &image, QWidget *parent = nullptr);

    QImage resultImage() const;
    void resetSelection();

signals:
    void selectionStarted();
    void reselectRequested();
    void captureRequested(ScreenCaptureController::Action action);
    void cancelRequested();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    enum Handle { NoHandle = -1, TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left };
    enum Drag { NoDrag, CreateSelection, MoveSelection, ResizeSelection };

    QRect physicalRect() const;
    QPointF boundedPoint(const QPointF &point) const;
    QSizeF minimumSelectionSize() const;
    QRectF newSelection(const QPointF &anchor, const QPointF &point) const;
    std::array<QPointF, 8> handlePositions() const;
    Handle hitHandle(const QPointF &point) const;
    bool containsSelection(const QPointF &point) const;
    void updateSelection(const QPointF &point);
    void updateCursor(const QPointF &point);
    void arrangeToolbar();
    void requestCapture(ScreenCaptureController::Action action);

    QImage image;
    QRectF selection;
    QRectF initialSelection;
    QPointF pressPoint;
    Drag drag = NoDrag;
    Handle handle = NoHandle;
    QWidget *toolbar = nullptr;
    QGridLayout *toolbarLayout = nullptr;
    std::array<QPushButton *, 4> buttons = {{nullptr, nullptr, nullptr, nullptr}};
};