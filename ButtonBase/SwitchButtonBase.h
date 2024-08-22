#pragma once

#include <QToolButton>
#include <QPropertyAnimation>
#include <QPainter>

class SwitchButtonBase : public QToolButton
{
    Q_OBJECT
        Q_PROPERTY(float indicatorX READ indicatorX WRITE setIndicatorX NOTIFY indicatorXChanged)

public:
    explicit SwitchButtonBase(QWidget* parent = nullptr);

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    float indicatorX() const;
    void setIndicatorX(float x);

    virtual void paintBackground(QPainter& painter) = 0;
    virtual void paintIndicator(QPainter& painter) = 0;

    virtual void animaParamsInit();

signals:
    void indicatorXChanged();

protected:
    QPropertyAnimation* slideAni;
    float _indicatorX;
    float start, end;
};
