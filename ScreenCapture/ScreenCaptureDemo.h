#pragma once

#include <QImage>
#include <QString>
#include <QWidget>

class QCloseEvent;
class QLabel;
class QPushButton;
class ScreenCaptureController;
class ScreenCapturePreview;

class ScreenCaptureDemo : public QWidget
{
public:
    explicit ScreenCaptureDemo(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void startCapture();
    void copyResult();
    void savePNG();
    void updateCaptureUi(bool active);
    void updateResultUi();
    void setStatus(const QString &text, bool error = false);
    void showSaveError(const QString &message);

    QImage m_result;
    ScreenCaptureController *m_controller = nullptr;
    ScreenCapturePreview *m_preview = nullptr;
    QPushButton *m_startButton = nullptr;
    QPushButton *m_copyButton = nullptr;
    QPushButton *m_saveButton = nullptr;
    QLabel *m_sizeLabel = nullptr;
    QLabel *m_statusLabel = nullptr;
    bool m_closing = false;
};