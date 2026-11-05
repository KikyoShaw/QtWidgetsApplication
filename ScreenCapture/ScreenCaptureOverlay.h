#pragma once

#include "ScreenCaptureController.h"
#include "ScreenCaptureAnnotation.h"

#include <QImage>
#include <QPointF>
#include <QRectF>
#include <QWidget>
#include <array>

class QButtonGroup;
class QCloseEvent;
class QComboBox;
class QEvent;
class QGridLayout;
class QLabel;
class QMouseEvent;
class QPaintEvent;
class QPlainTextEdit;
class QPushButton;
class QResizeEvent;
class QScrollArea;
class QShortcut;

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
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    enum Handle { NoHandle = -1, TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left };
    enum Drag { NoDrag, CreateSelection, MoveSelection, ResizeSelection };
    enum class Tool { Select, Mosaic, Text, Arrow };

    QRect physicalRect() const;
    QRect toPhysicalRect(const QRectF &logicalRect) const;
    QPointF toPhysicalPoint(const QPointF &point) const;
    qreal physicalSizeRatio() const;
    QPointF boundedPoint(const QPointF &point) const;
    QPointF boundedSelectionPoint(const QPointF &point) const;
    QSizeF minimumSelectionSize() const;
    QRectF newSelection(const QPointF &anchor, const QPointF &point) const;
    std::array<QPointF, 8> handlePositions() const;
    Handle hitHandle(const QPointF &point) const;
    bool containsSelection(const QPointF &point) const;
    void updateSelection(const QPointF &point);
    void updateCursor(const QPointF &point);
    void arrangeToolbar();
    void showToolbar();
    void setTool(Tool nextTool);
    void updateToolControls();
    QColor annotationColor() const;
    void rebuildComposite();
    void commitAnnotation(const ScreenCaptureAnnotations::Annotation &annotation);
    void updateTemporaryAnnotation(const QPointF &point);
    void beginTextEditing(const QPointF &point);
    void updateTextEditor();
    void fitTextEditor();
    void finishEditing(bool commitText, bool cancelTemp);
    void undoAnnotation();
    void clearAnnotations();
    void requestCancel(bool cancelSession = false);
    void requestCapture(ScreenCaptureController::Action action);

    QImage image;
    QImage composite;
    QVector<ScreenCaptureAnnotations::Annotation> annotations;
    ScreenCaptureAnnotations::Annotation temporaryAnnotation;
    bool temporaryActive = false;
    QImage temporaryPatch;
    QPoint temporaryOrigin;
    QRectF selection;
    QRectF initialSelection;
    QPointF pressPoint;
    Drag drag = NoDrag;
    Handle handle = NoHandle;
    Tool tool = Tool::Select;
    QWidget *toolbar = nullptr;
    QWidget *toolbarContent = nullptr;
    QScrollArea *toolbarScrollArea = nullptr;
    QGridLayout *toolbarLayout = nullptr;
    QButtonGroup *toolGroup = nullptr;
    std::array<QPushButton *, 4> toolButtons = {{nullptr, nullptr, nullptr, nullptr}};
    std::array<QPushButton *, 4> buttons = {{nullptr, nullptr, nullptr, nullptr}};
    QPushButton *undoButton = nullptr;
    QPushButton *clearButton = nullptr;
    QLabel *colorLabel = nullptr;
    QLabel *fontSizeLabel = nullptr;
    QLabel *lineWidthLabel = nullptr;
    QLabel *mosaicSizeLabel = nullptr;
    QComboBox *colorCombo = nullptr;
    QComboBox *fontSizeCombo = nullptr;
    QComboBox *lineWidthCombo = nullptr;
    QComboBox *mosaicSizeCombo = nullptr;
    QPlainTextEdit *textEditor = nullptr;
    QRect textClip;
    bool textEditing = false;
    bool finishingEditing = false;
    bool fittingTextEditor = false;
    QShortcut *copyShortcut = nullptr;
    QShortcut *keypadCopyShortcut = nullptr;
    QShortcut *undoShortcut = nullptr;
};