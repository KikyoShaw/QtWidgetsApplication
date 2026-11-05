#pragma once

#include <QObject>
#include <QImage>
#include <memory>

class QWidget;

class ScreenCaptureController : public QObject
{
    Q_OBJECT

public:
    enum Action { Copy, Save };
    Q_ENUM(Action)

    explicit ScreenCaptureController(QObject *parent = nullptr);
    ~ScreenCaptureController() override;

    bool isActive() const;
    bool start(QWidget *sourceWindow);

public slots:
    void cancel();

signals:
    void captured(const QImage &image, ScreenCaptureController::Action action);
    void cancelled();
    void error(const QString &message);

private:
    class Private;
    std::unique_ptr<Private> d;
};