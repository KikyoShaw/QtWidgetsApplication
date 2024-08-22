#include "SwitchButtonBase.h"

SwitchButtonBase::SwitchButtonBase(QWidget* parent)
	: QToolButton(parent), _indicatorX(0.0f), start(0), end(0)
{
	setCheckable(true);

	slideAni = new QPropertyAnimation(this, "indicatorX");
	slideAni->setDuration(200);
}

void SwitchButtonBase::mouseReleaseEvent(QMouseEvent* event)
{
	QToolButton::mouseReleaseEvent(event);
	if (isChecked())
		slideAni->setEndValue(end);
	else
		slideAni->setEndValue(start);
	slideAni->start();
}

void SwitchButtonBase::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	paintBackground(painter);
	paintIndicator(painter);
}

void SwitchButtonBase::resizeEvent(QResizeEvent* event)
{
	QToolButton::resizeEvent(event);
	animaParamsInit();
}

float SwitchButtonBase::indicatorX() const
{
	return _indicatorX;
}

void SwitchButtonBase::setIndicatorX(float x)
{
	_indicatorX = x;
	update();
	emit indicatorXChanged();
}

void SwitchButtonBase::animaParamsInit()
{
	start = height() / 2;
	end = width() - 2 - start;
	_indicatorX = start;
}
