#ifndef WATERZOOMBUTTON_H
#define WATERZOOMBUTTON_H

#include <QObject>
#include "interactivebuttonbase.h"

class WaterZoomButton : public InteractiveButtonBase
{
public:
    WaterZoomButton(QString text = "", QWidget* parent = nullptr);

    void setChoking(int c);
    void setChokingProp(double p);
    void setRadiusZoom(int radius);
    void setRadius(int x, int x2);

protected:
    QPainterPath getBgPainterPath() override;
    void resizeEvent(QResizeEvent *event) override;

protected:
    int choking; // 向里缩的长度
    double choking_prop;
    int radius_zoom; // 收缩时的半径
};

#endif // WATERZOOMBUTTON_H
