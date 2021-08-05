#include "rollingBox.h"
#include <QPainter>
#include <QWheelEvent>

RollingBox::RollingBox(QWidget *parent) 
	: QWidget(parent), m_nCurrentValue(0), m_nOffSet(0), m_nMax(0)
	, m_nMin(0), m_nDevice(7), m_nStep(1)
{

}

RollingBox::~RollingBox()
{

}

void RollingBox::setRang(int nMin, int nMax)
{
	m_nMin = nMin;
	m_nMax = nMax;
	if (m_nCurrentValue > m_nMax) {
		setValue(m_nMax);
	}
	update();
	//m_nCurrentValue = m_nCurrentValue > m_nMax ? m_nMax : m_nCurrentValue;
	//m_nCurrentValue = m_nCurrentValue < m_nMin ? m_nMin : m_nCurrentValue;
}

void RollingBox::wheelEvent(QWheelEvent *event)
{
	//滚动的角度,*8就是鼠标滚动的距离
	int nDegrees = event->delta() / 8;
	//滚动的步数,*15就是鼠标滚动的角度
	int nSteps = nDegrees / 15;
	if (0 != m_nDevice * nSteps) {
		m_nOffSet = height() / m_nDevice * nSteps;
	}
	if (m_nMin == m_nMax){
		return;
	}
	//在绘制界面之前我们需要通过偏移量来计算当前值
	if (m_nOffSet >= (height() / m_nDevice) && m_nCurrentValue > m_nMin)
	{
		m_nOffSet -= height() / m_nDevice;
		setValue(m_nCurrentValue - m_nStep);
	}
	else if (m_nOffSet <= -height() / m_nDevice && m_nCurrentValue < m_nMax)
	{
		m_nOffSet += height() / m_nDevice;
		setValue(m_nCurrentValue + m_nStep);
	}

	if (m_nCurrentValue == m_nMax || m_nCurrentValue == m_nMin){
		m_nOffSet = 0;
	}
	//发送修改信号
	emit sigCurrentValueChange(m_nCurrentValue);
	update();
}

void RollingBox::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	// 反锯齿;
	painter.setRenderHint(QPainter::Antialiasing);
	//平滑边缘处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	//绘制线条
	paintLine(&painter);
	int nFontSize = 16;
	//绘制字体
	paintText(&painter, m_nCurrentValue, m_nOffSet, nFontSize);
	//绘制两边的字体
	//两边字体的偏移量是通过距离计算的
	for (int i = 1; i <= m_nDevice / 2; ++i){
		nFontSize -= 2;
		if (m_nCurrentValue - m_nStep * i >= m_nMin){
			paintText(&painter, m_nCurrentValue - m_nStep * i, m_nOffSet - height() / m_nDevice * i, nFontSize);
		}
		if (m_nCurrentValue + m_nStep * i <= m_nMax){
			paintText(&painter, m_nCurrentValue + m_nStep * i, m_nOffSet + height() / m_nDevice * i, nFontSize);
		}
	}
}

void RollingBox::paintLine(QPainter *pPainter)
{
	pPainter->save();
	pPainter->setPen(Qt::NoPen);
	auto posHeight = height() / m_nDevice;
	int posY = posHeight * 3;
	//画矩形
	const qreal radius = 6;
	QRectF r = QRect(10, posY, width() - 20, posHeight);
	QPainterPath path;
	path.moveTo(r.bottomRight() - QPointF(0, radius));
	path.lineTo(r.topRight() + QPointF(0, radius));
	path.arcTo(QRectF(QPointF(r.topRight() - QPointF(radius * 2, 0)), QSize(radius * 2, radius * 2)), 0, 90);
	path.lineTo(r.topLeft() + QPointF(radius, 0));
	path.arcTo(QRectF(QPointF(r.topLeft()), QSize(radius * 2, radius * 2)), 90, 90);
	path.lineTo(r.bottomLeft() - QPointF(0, radius));
	path.arcTo(QRectF(QPointF(r.bottomLeft() - QPointF(0, radius * 2)), QSize(radius * 2, radius * 2)), 180, 90);
	path.lineTo(r.bottomLeft() + QPointF(radius, 0));
	path.arcTo(QRectF(QPointF(r.bottomRight() - QPointF(radius * 2, radius * 2)), QSize(radius * 2, radius * 2)), 270, 90);
	// 背景底色
	QColor colorBrushFunLevel1 = QColor(255, 255, 255, 50);
	QColor colorBrushFunLevel2 = QColor(255, 255, 255, 50);
	QLinearGradient linearGradient(r.topLeft(), r.topRight());
	linearGradient.setColorAt(0.0, colorBrushFunLevel2);
	linearGradient.setColorAt(1, colorBrushFunLevel1);
	pPainter->setBrush(QBrush(linearGradient));
	pPainter->drawPath(path);
	pPainter->restore();
}

void RollingBox::paintText(QPainter *pPainter, int nValue, int nOffSet, int nFontSize)
{
	pPainter->save();
	QFont font(QStringLiteral("微软雅黑"));
	int size = nOffSet == 0 ? 16 : 12;
	font.setPixelSize(size);
	QColor nColor = nOffSet == 0 ? QColor(1, 238, 195) : QColor(255,255,255,180);
	QPen pen = pPainter->pen();
	pen.setColor(nColor);
	pPainter->setPen(pen);
	pPainter->setBrush(Qt::NoBrush);
	pPainter->setFont(font);
	int textHeight = pPainter->fontMetrics().height();
	int initY = height() / 2 + nOffSet - textHeight / 2 - 2;
	//拼接文字
	auto _text = QString::number(nValue);
	if (nValue < 10) {
		_text = QString("0%1").arg(_text);
	}
	pPainter->drawText(QRect(0, initY, width(), textHeight), Qt::AlignCenter, _text);
	pPainter->restore();
}
