#include "paintEvent.h"
#include <QPainter>
#include <QVariantAnimation>

paintEvents::paintEvents(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	m_vAnimation = new QVariantAnimation(this);
	if (m_vAnimation) {
		m_vAnimation->setDuration(2000);
		m_vAnimation->setStartValue(0);
		m_vAnimation->setEndValue(3);
		m_vAnimation->setLoopCount(-1);
		m_vAnimation->start();
		connect(m_vAnimation, &QVariantAnimation::valueChanged, this, &paintEvents::sltAtIndexChanged);
	}
}

void paintEvents::sltAtIndexChanged(QVariant value)
{
	m_nAtIndex = value.toDouble();
	update();
}

void paintEvents::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

	//绘制心
	//paintHeart(&painter);

	//绘制特殊文本
	paintColorText(&painter);
}

void paintEvents::paintHeart(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	double k = 10;
	QColor my_color(237, 162, 255, 255);
	QBrush my_brush(my_color);
	painter->setPen(Qt::red);
	painter->translate(this->width() / 2, this->height() / 2);
	QPainterPath polygonPath;
	polygonPath.setFillRule(Qt::WindingFill);
	float x = 16 * k * sin(0.0)*sin(0.0)*sin(0.0);
	float y = 13 * k * cos(0.0) - 5 * k*cos(0.0) - 2 * k*cos(0.0) - cos(0.0);
	polygonPath.moveTo(x, -y);
	for (double t = 0.01; t < 100; t += 0.05) {
		x = 16 * k * sin(k*t)*sin(k*t)*sin(k*t);
		y = 13 * k * cos(k*t) - 5 * k * cos(2 * k * t) - 2 * k * cos(3 * k * t) - cos(4 * k * t);
		polygonPath.lineTo(x, -y);
		//painter->drawPoint(x,-y);
		//painter->drawLine(0,0,x,-y);
		//painter->drawLine(0,0,x+100,-y);
	}
	painter->drawPath(polygonPath);
}

void paintEvents::paintColorText(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	QFont font = painter->font();
	font.setPixelSize(200);

	QRadialGradient g(width() / 2, height() / 2, 100);
	g.setSpread(QGradient::ReflectSpread);
	//double s = 6;
	//g.setColorAt(0 / s, Qt::yellow);
	//g.setColorAt(1 / s, Qt::green);
	//g.setColorAt(2 / s, Qt::blue);
	//g.setColorAt(3 / s, Qt::red);
	//g.setColorAt(4 / s, Qt::magenta);
	//g.setColorAt(5 / s, Qt::cyan);
	//g.setColorAt(6 / s, Qt::white);
	if (0 == m_nAtIndex) {
		g.setColorAt(1, Qt::yellow);
		g.setColorAt(0, Qt::yellow);
	}
	else if (1 == m_nAtIndex) {
		g.setColorAt(1, Qt::green);
		g.setColorAt(0, Qt::green);
	}
	else if (2 == m_nAtIndex) {
		g.setColorAt(1, Qt::blue);
		g.setColorAt(0, Qt::blue);
	}
	else if (3 == m_nAtIndex) {
		g.setColorAt(1, Qt::red);
		g.setColorAt(0, Qt::red);
	}

	QString str = QStringLiteral("十三先生");
	QFontMetrics fm(font);
	QRect rect = fm.boundingRect(this->rect(), Qt::AlignCenter, str);

	QPainterPath path;
	path.addText(rect.x(), rect.y() + fm.ascent(), font, str);

	painter->setOpacity(0.5);
	painter->fillPath(path, Qt::black);
	painter->strokePath(path, QPen(Qt::black, 6));

	painter->save();
	painter->setOpacity(1);
	painter->translate(-4, -3);
	painter->fillPath(path, g);
	painter->strokePath(path, QPen(Qt::darkMagenta, 4));
	painter->restore();

	painter->setOpacity(0.1);
	painter->fillRect(rect, g);
}
