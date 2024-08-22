#include "SwitchButton.h"

SwitchButton::SwitchButton(QWidget* parent)
	: SwitchButtonBase(parent)
{
    eSwitchButtonType = emSwitchButtonType::V1;
    indicator_color = QColor(255, 255, 255);
    background_color = QColor(188, 188, 188);
    checked_background_color = QColor(0, 89, 89);
    checked_indicator_color = QColor(255, 255, 255);
}

void SwitchButton::setParams(const QColor& indicatorColor, const QColor& backgroundColor, const QColor& checkedBackgroundColor, const QColor& checkedIndicatorColor)
{
    indicator_color = indicatorColor;
    background_color = backgroundColor;
    checked_background_color = checkedBackgroundColor;
    checked_indicator_color = checkedIndicatorColor;
}

void SwitchButton::setSwitchButtonType(emSwitchButtonType eType)
{
    eSwitchButtonType = eType;
}

void SwitchButton::paintBackground(QPainter& painter)
{
    painter.save();
    switch (eSwitchButtonType)
    {
    case V1:
        paintBackgroundV1(painter);
        break;
    case V2:
        paintBackgroundV2(painter);
        break;
    case V3:
        paintBackgroundV3(painter);
        break;
    default:
        break;
    }
    painter.restore();
}

void SwitchButton::paintIndicator(QPainter& painter)
{
    painter.save();
    switch (eSwitchButtonType)
    {
    case V1:
        paintIndicatorV1(painter);
        break;
    case V2:
        paintIndicatorV2(painter);
        break;
    case V3:
        paintIndicatorV3(painter);
        break;
    default:
        break;
    }
    painter.restore();
}

void SwitchButton::paintBackgroundV1(QPainter& painter)
{
    int r = height() / 2;
    QRect rect(1, 1, width() - 2, height() - 1);

    if (!isChecked())
        painter.setBrush(background_color);
    else
        painter.setBrush(checked_background_color);

    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, r, r);
}

void SwitchButton::paintIndicatorV1(QPainter& painter)
{
    int r = height() / 3;
    QPoint point(static_cast<int>(_indicatorX), 0);

    painter.translate(0, height() / 2);

    if (isChecked())
        painter.setBrush(checked_indicator_color);
    else
        painter.setBrush(indicator_color);

    painter.setPen(Qt::NoPen);
    painter.drawEllipse(point, r, r);
}

void SwitchButton::paintBackgroundV2(QPainter& painter)
{
    int r = height() / 2;
    QRect rect(1, 1, width() - 2, height() - 2);

    QPen pen;
    pen.setWidth(3);
    if (isChecked())
    {
        pen.setColor(checked_background_color);
        painter.setPen(pen);
    }
    else
    {
        pen.setColor(background_color);
        painter.setPen(pen);
    }

    painter.drawRoundedRect(rect, r, r);
}

void SwitchButton::paintIndicatorV2(QPainter& painter)
{
    int r = height() / 3;
    QPoint point(static_cast<int>(_indicatorX), 0);

    painter.translate(0, height() / 2);

    if (isChecked())
        painter.setBrush(checked_indicator_color);
    else
        painter.setBrush(indicator_color);

    painter.setPen(Qt::NoPen);
    painter.drawEllipse(point, r, r);
}

void SwitchButton::paintBackgroundV3(QPainter& painter)
{
    int w = width() - width() / 3;
    int h = height() - height() / 3;
    QRect rect(-w / 2, -h / 2, w, h);
    int r = h / 2;

    painter.translate(width() / 2, height() / 2);

    if (isChecked())
        painter.setBrush(checked_background_color);
    else
        painter.setBrush(background_color);

    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect, r, r);
}

void SwitchButton::paintIndicatorV3(QPainter& painter)
{
    int r = (height() - height() / 3) / 2 + height() / 9;
    QPoint point(static_cast<int>(_indicatorX), 0);

    painter.translate(0, height() / 2);

    if (isChecked())
        painter.setBrush(checked_indicator_color);
    else
        painter.setBrush(indicator_color);

    painter.setPen(Qt::NoPen);
    painter.drawEllipse(point, r, r);
}
