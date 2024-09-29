#include "vWaterProgressBar.h"
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QTimerEvent>
#include <QLinearGradient>
#include <qmath.h>
#include <QBrush>

vWaterProgressBar::vWaterProgressBar(QWidget *parent /* = nullptr */)
	:QWidget(parent)
{
	m_font.setFamily("Microsoft YaHei");
	this->startTimer(80);
}

vWaterProgressBar::~vWaterProgressBar()
{
}

void vWaterProgressBar::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	drawValue(&painter);
}

void vWaterProgressBar::timerEvent(QTimerEvent * event)
{
	Q_UNUSED(event);
	this->update();
}

void vWaterProgressBar::drawValue(QPainter * painter)
{
	Q_UNUSED(painter);
	// 反锯齿;
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);
	
	int height = this->height();
	int width = this->width();
	int side = qMin(width, height);
	
	//计算当前值所占百分比
	double percent = 1 - (double)(m_value - m_minValue) / (m_maxValue - m_minValue);
	
	//正弦曲线公式 y = A * sin(ωx + φ) + k
	//w表示周期,可以理解为水波的密度,值越大密度越大(浪越密集 ^_^),取值 密度*M_PI/宽度
	double w = 0.038;//m_waterDensity * M_PI * 1.6/ width;
	
	//A表示振幅,可以理解为水波的高度,值越大高度越高(越浪 ^_^),取值高度的百分比
	double A = height * m_waterHeight;
	
	//k表示y轴偏移,可以理解为进度,取值高度的进度百分比
	double k = height * percent;
	
	//第一条波浪路径集合
	QPainterPath waterPath1;
	
	//第二条波浪路径集合
	QPainterPath waterPath2;
	
	//移动到左上角起始点
	waterPath1.moveTo(0, height);
	waterPath2.moveTo(0, height);
	
	m_offset += 0.6;
	if (m_offset > (width / 2)) {
		m_offset = 0;
	}

	double waterY1 = 0.0;
	double waterY2 = 0.0;
	for (int x = 0; x <= width; x++) {
		//第一条波浪Y轴
		waterY1 = (double)(A * qSin(w * x + m_offset)) + k;
		//第二条波浪Y轴
		waterY2 = (double)(A * qSin(w * x + m_offset + (width / 2 * w))) + k;
		//如果当前值为最小值则Y轴为高度
		if (m_value == m_minValue) {
			waterY1 = height;
			waterY2 = height;
		}
		//如果当前值为最大值则Y轴为0
		if (m_value == m_maxValue) {
			waterY1 = 0;
			waterY2 = 0;
		}
		waterPath1.lineTo(x, waterY1);
		waterPath2.lineTo(x, waterY2);
	}

	//移动到右下角结束点,整体形成一个闭合路径
	waterPath1.lineTo(width, height);
	waterPath2.lineTo(width, height);
	
	//画背景，此处由单一色调改为了渐变色
	QRadialGradient radialGradient(side / 2, height / 2, side / 2, side / 2, side / 2);
	radialGradient.setColorAt(0, QColor(0, 0, 255, 100));
	radialGradient.setColorAt(1.0, QColor(50, 50, 255, 200));
	
	QPainterPath bigPath;
	if (m_percentStyle == PercentStyle_Rect) {
		width = width - m_borderWidth * 2;
		height = height - m_borderWidth * 2;
		bigPath.addRect(m_borderWidth, m_borderWidth, width, height);
		//画边框
		if (0 < m_borderWidth) {
			painter->setPen(m_boderColor);
			//painter->drawRect(m_borderWidth, m_borderWidth, width, height);
		}
		painter->setBrush(m_bgColor);
		//painter->setBrush(QBrush(radialGradient));
		//painter->drawRect(this->rect());
		painter->drawRect(m_borderWidth, m_borderWidth, width, height);
	}	
	else if (m_percentStyle == PercentStyle_Circle) {
		/*painter->setBrush(m_boderColor);
        painter->setBrush(QBrush(radialGradient));
        painter->drawEllipse((width - side) / 2, (height - side) / 2, side, height);*/
		side = side - m_borderWidth * 2;
		bigPath.addEllipse((width - side) / 2, m_borderWidth, side, side);
		if (0 < m_borderWidth) {
			painter->setPen(m_boderColor);
		}
		painter->setBrush(m_bgColor);
		//painter->setBrush(QBrush(radialGradient));
		painter->drawEllipse((width - side) / 2, m_borderWidth, side, side);
	}
	else if (m_percentStyle == PercentStyle_Ellipse) {
		width = width - m_borderWidth * 2;
		height = height - m_borderWidth * 2;
		bigPath.addEllipse(m_borderWidth, m_borderWidth, width, height);
		if (0 < m_borderWidth) {
			painter->setPen(m_boderColor);
			//painter->setBrush(QBrush(radialGradient));
			//painter->drawEllipse(m_borderWidth, m_borderWidth, width, height);
		}
		painter->setBrush(m_bgColor);
		//painter->setBrush(QBrush(radialGradient));
		//painter->drawEllipse(this->rect());
		painter->drawEllipse(m_borderWidth, m_borderWidth, width, height);
	}
	painter->save();
	
	//新路径,用大路径减去波浪区域的路径,形成遮罩效果
	QPainterPath path;
	painter->setPen(Qt::NoPen);
	QColor waterColor1 = m_frontColor;
	waterColor1.setAlpha(100);
	QColor waterColor2 = m_frontColor;
	waterColor2.setAlpha(180);
	
	//使用渐变色
	QLinearGradient linearGradient1(0, 0, 0, side);
	linearGradient1.setColorAt(0.0, QColor(255, 66, 213, 100));
	linearGradient1.setColorAt(1.0, QColor(43, 74, 255, 100));
	//linearGradient1.setColorAt(0.2, QColor(0, 255, 255, 100));
	//linearGradient1.setColorAt(0.6, QColor(50, 200, 50, 100));
	//linearGradient1.setColorAt(1.0, QColor(25, 150, 25, 100));
	
	QLinearGradient linearGradient2(0, 0, 0, side);
	linearGradient2.setColorAt(0.0, QColor(255, 66, 213, 180));
	linearGradient2.setColorAt(1.0, QColor(43, 74, 255, 180));
	//linearGradient2.setColorAt(0.2, QColor(0, 255, 255, 180));
	//linearGradient2.setColorAt(0.6, QColor(50, 200, 50, 180));
	//linearGradient2.setColorAt(1.0, QColor(25, 150, 25, 180));
	
	//第一条波浪挖去后的路径
	path = bigPath.intersected(waterPath1);
	painter->setBrush(QBrush(linearGradient1));
	painter->drawPath(path);
	
	//第二条波浪挖去后的路径
	path = bigPath.intersected(waterPath2);
	painter->setBrush(QBrush(linearGradient2));
	painter->drawPath(path);
	painter->restore();
	
	//绘制文字
	m_font.setPixelSize(this->width() / 8);
	painter->setPen(m_textColor);
	painter->setFont(m_font);
	painter->drawText(this->rect(), Qt::AlignCenter, QString("%0%").arg(QString::number(m_value)));
}
