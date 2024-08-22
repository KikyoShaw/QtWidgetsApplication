#pragma once

#include "SwitchButtonBase.h"

enum emSwitchButtonType {
    V1 = 0,
    V2 = 1,
    V3 = 2,
};

class SwitchButton : public SwitchButtonBase
{
    Q_OBJECT

public:
    explicit SwitchButton(QWidget* parent = nullptr);

    void setParams(const QColor& indicatorColor = QColor(255, 255, 255),
        const QColor& backgroundColor = QColor(188, 188, 188),
        const QColor& checkedBackgroundColor = QColor(0, 89, 89),
        const QColor& checkedIndicatorColor = QColor(255, 255, 255));

    void setSwitchButtonType(emSwitchButtonType eType);


protected:
    void paintBackground(QPainter& painter) override;
    void paintIndicator(QPainter& painter) override;

private:
    void paintBackgroundV1(QPainter& painter);
    void paintIndicatorV1(QPainter& painter);

    void paintBackgroundV2(QPainter& painter);
    void paintIndicatorV2(QPainter& painter);

    void paintBackgroundV3(QPainter& painter);
    void paintIndicatorV3(QPainter& painter);

private:
    QColor indicator_color;
    QColor background_color;
    QColor checked_background_color;
    QColor checked_indicator_color;
    emSwitchButtonType eSwitchButtonType;
};

