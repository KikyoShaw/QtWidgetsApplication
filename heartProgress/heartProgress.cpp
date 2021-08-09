#include "heartProgress.h"
#include <QPainter>

heartProgress::heartProgress(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

heartProgress::~heartProgress()
{
}

void heartProgress::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	// ¿¹¾â³Ý
	painter.setRenderHints(QPainter::Antialiasing, true);
	// Æ½»¬´¦Àí
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	double k = 10;
	QColor my_color(237, 162, 255, 255);
	QBrush my_brush(my_color);
	painter.setPen(Qt::red);
	painter.translate(this->width() / 2, this->height() / 2);
	QPainterPath polygonPath;
	polygonPath.setFillRule(Qt::WindingFill);
	float x = 16 * k * sin(0.0)*sin(0.0)*sin(0.0);
	float y = 13 * k * cos(0.0) - 5 * k*cos(0.0) - 2 * k*cos(0.0) - cos(0.0);
	polygonPath.moveTo(x, -y);
	for (double t = 0.01; t < 100; t += 0.05){
		x = 16 * k * sin(k*t)*sin(k*t)*sin(k*t);
		y = 13 * k * cos(k*t) - 5 * k * cos(2 * k * t) - 2 * k * cos(3 * k * t) - cos(4 * k * t);
		polygonPath.lineTo(x, -y);
		//painter.drawPoint(x,-y);
		//painter.drawLine(0,0,x,-y);
		//painter.drawLine(0,0,x+100,-y);
	}
	painter.drawPath(polygonPath);
}

void heartProgress::paintBg(QPainter * painter)
{

}

void heartProgress::paintText(QPainter * painter)
{

}
