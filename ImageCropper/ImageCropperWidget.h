#pragma once

#include <QImage>
#include <QRect>
#include <QSize>
#include <QWidget>
#include <memory>

class ImageCropperWidget : public QWidget
{
    Q_OBJECT

public:
    enum AspectMode { Free, Square, Ratio4x3, Ratio16x9, Circle };
    Q_ENUM(AspectMode)

    explicit ImageCropperWidget(QWidget *parent = nullptr);
    ~ImageCropperWidget() override;

    void setImage(const QImage &image);
    QSize imageSize() const;
    QRect cropRect() const;
    qreal zoomFactor() const;
    AspectMode aspectMode() const;
    bool isAspectModeAvailable(AspectMode mode) const;
    bool setAspectMode(AspectMode mode);
    QImage preview(const QSize &maximumSize) const;
    QImage croppedImage() const;

public slots:
    void fitToWindow();
    void reset();

signals:
    void imageChanged();
    void cropChanged(const QRect &rect);
    void zoomChanged(qreal factor);
    void aspectModeChanged(ImageCropperWidget::AspectMode mode);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    bool event(QEvent *event) override;

private:
    struct Private;
    std::unique_ptr<Private> d;
};