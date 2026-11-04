#pragma once

#include <QString>
#include <QWidget>

class ImageCropperWidget;
class ImageCropperPreview;
class QComboBox;
class QDragEnterEvent;
class QDropEvent;
class QLabel;
class QPushButton;

class ImageCropperDemo : public QWidget
{
public:
    explicit ImageCropperDemo(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void openImage();
    void loadImage(const QString &path);
    void useSampleImage();
    void exportImage();
    void updateImageUi();
    void updateCropUi();
    void updateAspectUi();
    void updateZoomUi();
    void setStatus(const QString &text, bool error = false);

    ImageCropperWidget *m_cropper = nullptr;
    ImageCropperPreview *m_preview = nullptr;
    QComboBox *m_aspectCombo = nullptr;
    QPushButton *m_fitButton = nullptr;
    QPushButton *m_resetButton = nullptr;
    QPushButton *m_exportButton = nullptr;
    QLabel *m_sourceLabel = nullptr;
    QLabel *m_originalSizeLabel = nullptr;
    QLabel *m_cropLabel = nullptr;
    QLabel *m_zoomLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    QString m_sourcePath;
};