#include "rollingBox.h"
#include <QPainter>
#include <QWheelEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QColor>


RollingBox::RollingBox(QWidget *parent)
	: QWidget(parent), m_nCurrentValue(0), m_nOffSet(0), m_nMax(0)
	, m_nMin(0), m_nDevice(5), m_nStep(1), m_dateProperty(E_Property_None)
	, m_scrollType(E_VERTICAL)
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
	//�����ĽǶ�,*8�����������ľ���
	auto nDegrees = event->delta() / 8;
	//�����Ĳ���,*15�����������ĽǶ�
	auto nSteps = nDegrees / 15;
	auto nTarget = m_scrollType == ScrollType::E_VERTICAL ? this->height() : this->width();
	if (0 != m_nDevice * nSteps) {
		m_nOffSet = nTarget / m_nDevice * nSteps;
	}
	if (m_nMin == m_nMax) {
		return;
	}
	//�ڻ��ƽ���֮ǰ������Ҫͨ��ƫ���������㵱ǰֵ
	if (m_nOffSet >= (nTarget / m_nDevice) && m_nCurrentValue > m_nMin)
	{
		m_nOffSet -= nTarget / m_nDevice;
		setValue(m_nCurrentValue - m_nStep);
	}
	else if (m_nOffSet <= -nTarget / m_nDevice && m_nCurrentValue < m_nMax)
	{
		m_nOffSet += nTarget / m_nDevice;
		setValue(m_nCurrentValue + m_nStep);
	}

	if (m_nCurrentValue == m_nMax || m_nCurrentValue == m_nMin) {
		m_nOffSet = 0;
	}
	//�����޸��ź�
	emit sigCurrentValueChange(m_nCurrentValue, m_dateProperty);
	update();
}

void RollingBox::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	// �����;
	painter.setRenderHint(QPainter::Antialiasing);
	//ƽ����Ե����
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	//��������
	paintLine(&painter);
	int nFontSize = 16;
	//��������
	paintText(&painter, m_nCurrentValue, m_nOffSet, nFontSize);
	//�������ߵ�����
	int nTarget = m_scrollType == ScrollType::E_VERTICAL ? this->height() : this->width();
	//���������ƫ������ͨ����������
	for (int i = 1; i <= m_nDevice / 2; ++i) {
		nFontSize -= 2;
		if (m_nCurrentValue - m_nStep * i >= m_nMin) {
			paintText(&painter, m_nCurrentValue - m_nStep * i, m_nOffSet - nTarget / m_nDevice * i, nFontSize);
		}
		if (m_nCurrentValue + m_nStep * i <= m_nMax) {
			paintText(&painter, m_nCurrentValue + m_nStep * i, m_nOffSet + nTarget / m_nDevice * i, nFontSize);
		}
	}
}

void RollingBox::paintLine(QPainter *pPainter)
{
	pPainter->save();
	pPainter->setPen(Qt::NoPen);
	auto posHeight = height() / m_nDevice;
	int posY = posHeight * 2;
	const qreal radius = 0;
	QRectF r = QRect(0, posY, width(), posHeight);
	if (m_scrollType == ScrollType::E_HORIZONTAL){
		auto posW = width() / m_nDevice;
		auto posX = width() - (posW * (m_nDevice / 2 + 1));
		//qInfo() << "posW = " << posW << "posX = " << posX;
		r = QRect(posX + 5, 0, posW, height());
	}
	//������
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
	// ������ɫ
	QColor colorBrushFunLevel1 = QColor(234, 234, 234, 15);
	QColor colorBrushFunLevel2 = QColor(234, 234, 234, 15);
	QLinearGradient linearGradient(r.topLeft(), r.topRight());
	linearGradient.setColorAt(0.0, colorBrushFunLevel2);
	linearGradient.setColorAt(1, colorBrushFunLevel1);
	pPainter->setBrush(QBrush(linearGradient));
	pPainter->drawPath(path);
	//pPainter->drawRect(0, 0, width(), height());
	pPainter->restore();
}

void RollingBox::paintText(QPainter *pPainter, int nValue, int nOffSet, int nFontSize)
{
	pPainter->save();
	QFont font(QStringLiteral("΢���ź�"));
	//int size = nOffSet == 0 ? 16 : 12;
	font.setPixelSize(12);
	//QColor nColor = nOffSet == 0 ? QColor(1, 238, 195) : QColor(255,255,255,180);
	QColor nColor = getTextStyle(nOffSet);
	QPen pen = pPainter->pen();
	pen.setColor(nColor);
	pPainter->setPen(pen);
	pPainter->setBrush(Qt::NoBrush);
	pPainter->setFont(font);
	int textHeight = pPainter->fontMetrics().height();
	int initY = height() / 2 + nOffSet - textHeight / 2 - 2;

	//ƴ������
	auto _text = getTextByProperty(QString::number(nValue));
	if (m_scrollType == ScrollType::E_HORIZONTAL)
	{
		int textWidth = pPainter->fontMetrics().width(nValue);
		int initX = width() / 2 + nOffSet - textWidth / 2;
		pPainter->drawText(QRect(initX, 0, 15, height()), Qt::AlignCenter, QString::number(nValue));
	}
	else {
		//qInfo() << "nOffSet = " << nOffSet << "initY = " << initY << "_text" << _text;
		if (E_Property_Year == m_dateProperty) {
			pPainter->drawText(QRect(36, initY, width(), textHeight), Qt::AlignLeft, _text);
		}
		else if (E_Property_Day == m_dateProperty) {
			pPainter->drawText(QRect(-36, initY, width(), textHeight), Qt::AlignRight, _text);
		}
		else {
			pPainter->drawText(QRect(0, initY, width(), textHeight), Qt::AlignCenter, _text);
		}
	}
	pPainter->restore();
}

QString RollingBox::getTextByProperty(const QString &text)
{
	QString descText = text;
	switch (m_dateProperty)
	{
	case E_Property_None:
		break;
	case E_Property_Day: {
		int day = descText.toInt();
		if (day < 10) {
			descText = QString("0%1").arg(descText);
		}
		descText = QStringLiteral("%1��").arg(descText);
		break;
	}
	case E_Property_Month: {
		descText = QStringLiteral("%1��").arg(text);
		break;
	}
	case E_Property_Year: {
		descText = QStringLiteral("%1��").arg(text);
		break;
	}
	default:
		break;
	}
	return descText;
}

QColor RollingBox::getTextStyle(int nOffSet)
{
	auto nTarget = m_scrollType == ScrollType::E_VERTICAL ? this->height() : this->width();
	auto index = nTarget / m_nDevice;
	if (-index == nOffSet || index == nOffSet)
		return QColor(255, 255, 255, 102);
	else if (-(index * 2) == nOffSet || (index * 2) == nOffSet)
		return QColor(255, 255, 255, 25);
	return QColor(1, 238, 195);
}
