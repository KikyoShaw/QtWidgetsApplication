#include "WaterProgressBar.h"
#include <QTimer>
#include <QPainter>
#include <QColor>
#include <QtMath>

WaterProgressBar::WaterProgressBar(QWidget *parent)
{
	m_iBorderWidth = 0;
	m_waterColor.setRgb(43, 123, 234);
	m_backgroundColor.setRgb(255, 255, 255);
	m_borderColor.setRgb(120, 120, 120);
	m_textColor.setRgb(0, 0, 0);
	m_dOffset = 0;
	//利用定时器固定时间内刷新页面，使得水浪动起来
	m_timer = new QTimer(this);
	m_timer->setSingleShot(false);
	connect(m_timer, &QTimer::timeout, this, [=]() {
		if (this->isVisible())
		{
			//偏移量控制，每次绘制自加0.3，当超过一个正弦余弦2π周期时，就回退为0，加上定时器每50ms绘制一下，就触发了曲线的动态效果
			m_dOffset += 0.3;
			if (m_dOffset > 2 * M_PI)
			{
				m_dOffset = 0;
			}
			this->update();
		}
	});
	m_timer->start(50);
}

WaterProgressBar::~WaterProgressBar()
{
}

void WaterProgressBar::paintEvent(QPaintEvent *event)
{
	//进度条不可见的情况下就不重绘了
	if (!this->isVisible())
	{
		return;
	}
	m_iValue = this->value() < 0 ? 0 : this->value();//初始化时QT进度条的进度为-1，避免负进度的出现

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);//开启图和字体抗锯齿

	drawBackGround(&painter);//绘制背景
	drawWaterWave(&painter);//绘制水波
	drawText(&painter);//绘制进度文本
}

void WaterProgressBar::drawBackGround(QPainter* painter)
{
	int width = this->width();
	int height = this->height();
	if (m_iBorderWidth > 0)
	{
		//根据窗口的长宽最小值得到外部背景直径
		int max_diameter = qMin(width, height);
		painter->save();
		painter->setBrush(QBrush(m_borderColor));
		painter->setPen(Qt::NoPen);
		//绘制边框背景，然后用内部背景覆盖，即可得到边框
		painter->drawEllipse(0, 0, max_diameter, max_diameter);
		painter->restore();
	}
	painter->save();
	//根据窗口的长宽最小值减去边框厚度得到内部背景直径
	int min_diameter = qMin(width, height) - (2 * m_iBorderWidth);
	painter->setBrush(QBrush(m_backgroundColor));
	painter->setPen(Qt::NoPen);
	painter->drawEllipse(m_iBorderWidth, m_iBorderWidth, min_diameter, min_diameter);
	painter->restore();
}

void WaterProgressBar::drawWaterWave(QPainter* painter)
{
	int width = this->width();
	int height = this->height();
	//根据窗口的长宽最小值减去边框厚度得到直径
	int diameter = qMin(width, height) - (2 * m_iBorderWidth);

	//sincos曲线的波峰，波峰越大，水浪越高
	double waveHeight = 0.04*diameter;
	//sincos曲线的周期，周期越大，水浪越密
	double cycle = 2 * M_PI / diameter;
	//水的高度，可以理解为进度，注意由于我们的电脑坐标系是以左上角为原点，这和我们高中坐标系理解的不同
	double percent = (double)m_iValue / 100;
	double waterHeight = (1 - percent)*diameter + m_iBorderWidth;

	painter->save();
	QPainterPath totalPath;
	//加入圆形路径
	totalPath.addEllipse(m_iBorderWidth, m_iBorderWidth, diameter, diameter);

	//水波路径,先画浅色，再画深色
	QPainterPath water1;
	QPainterPath water2;

	water1.moveTo(m_iBorderWidth, m_iBorderWidth + diameter);
	water2.moveTo(m_iBorderWidth, m_iBorderWidth + diameter);

	//从左边起始点到右边结束点绘制两条波浪曲线
	for (int i = m_iBorderWidth; i <= m_iBorderWidth + diameter; i++)
	{
		double waterY1 = 0;
		double waterY2 = 0;

		if (m_iValue == 0 || m_iValue == 100)
		{
			waterY1 = waterY2 = waterHeight;
		}
		else
		{
			//第一条波浪Y轴
			waterY1 = (double)(waveHeight * qSin(cycle * (i - m_iBorderWidth) - M_PI / 2 + m_dOffset)) + waterHeight;//当正弦曲线前进π/2，sin的波峰和cos的波谷就对上了
			//第二条波浪Y轴
			waterY2 = (double)(waveHeight * qCos(cycle * (i - m_iBorderWidth) + m_dOffset)) + waterHeight;
		}
		water1.lineTo(i, waterY1);
		water2.lineTo(i, waterY2);

	}

	//封闭
	water1.lineTo(m_iBorderWidth + diameter, m_iBorderWidth + diameter);
	water2.lineTo(m_iBorderWidth + diameter, m_iBorderWidth + diameter);

	QPainterPath path;
	QColor waterColor1 = m_waterColor;
	waterColor1.setAlpha(100);
	QColor waterColor2 = m_waterColor;
	waterColor2.setAlpha(200);

	//第一条波浪
	path = totalPath.intersected(water1);
	painter->setBrush(waterColor1);
	painter->setPen(Qt::NoPen);
	painter->drawPath(path);
	painter->restore();
	painter->save();

	//第二条波浪挖去后的路径
	path = totalPath.intersected(water2);
	painter->setBrush(waterColor2);
	painter->setPen(Qt::NoPen);
	painter->drawPath(path);

	painter->restore();
}

void WaterProgressBar::drawText(QPainter* painter)
{
	painter->save();
	int width = this->width();
	int height = this->height();
	//根据窗口的长宽最小值减去边框厚度得到直径
	int diameter = qMin(width, height) - (2 * m_iBorderWidth);
	int fontSize = diameter / 5;
	//设置文本字体
	QFont font;
	font.setFamily("微软雅黑");
	font.setPixelSize(fontSize);
	font.setBold(true);
	//绘制文本
	painter->setFont(font);
	painter->setPen(m_textColor);
	painter->drawText(QRectF(m_iBorderWidth, m_iBorderWidth, diameter, diameter), Qt::AlignCenter, QString("%1%").arg(m_iValue));
	painter->restore();
}