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
	// �����;
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);
	
	int height = this->height();
	int width = this->width();
	int side = qMin(width, height);
	
	//���㵱ǰֵ��ռ�ٷֱ�
	double percent = 1 - (double)(m_value - m_minValue) / (m_maxValue - m_minValue);
	
	//�������߹�ʽ y = A * sin(��x + ��) + k
	//w��ʾ����,�������Ϊˮ�����ܶ�,ֵԽ���ܶ�Խ��(��Խ�ܼ� ^_^),ȡֵ �ܶ�*M_PI/���
	double w = 0.038;//m_waterDensity * M_PI * 1.6/ width;
	
	//A��ʾ���,�������Ϊˮ���ĸ߶�,ֵԽ��߶�Խ��(Խ�� ^_^),ȡֵ�߶ȵİٷֱ�
	double A = height * m_waterHeight;
	
	//k��ʾy��ƫ��,�������Ϊ����,ȡֵ�߶ȵĽ��Ȱٷֱ�
	double k = height * percent;
	
	//��һ������·������
	QPainterPath waterPath1;
	
	//�ڶ�������·������
	QPainterPath waterPath2;
	
	//�ƶ������Ͻ���ʼ��
	waterPath1.moveTo(0, height);
	waterPath2.moveTo(0, height);
	
	m_offset += 0.6;
	if (m_offset > (width / 2)) {
		m_offset = 0;
	}

	double waterY1 = 0.0;
	double waterY2 = 0.0;
	for (int x = 0; x <= width; x++) {
		//��һ������Y��
		waterY1 = (double)(A * qSin(w * x + m_offset)) + k;
		//�ڶ�������Y��
		waterY2 = (double)(A * qSin(w * x + m_offset + (width / 2 * w))) + k;
		//�����ǰֵΪ��Сֵ��Y��Ϊ�߶�
		if (m_value == m_minValue) {
			waterY1 = height;
			waterY2 = height;
		}
		//�����ǰֵΪ���ֵ��Y��Ϊ0
		if (m_value == m_maxValue) {
			waterY1 = 0;
			waterY2 = 0;
		}
		waterPath1.lineTo(x, waterY1);
		waterPath2.lineTo(x, waterY2);
	}

	//�ƶ������½ǽ�����,�����γ�һ���պ�·��
	waterPath1.lineTo(width, height);
	waterPath2.lineTo(width, height);
	
	//���������˴��ɵ�һɫ����Ϊ�˽���ɫ
	QRadialGradient radialGradient(side / 2, height / 2, side / 2, side / 2, side / 2);
	radialGradient.setColorAt(0, QColor(0, 0, 255, 100));
	radialGradient.setColorAt(1.0, QColor(50, 50, 255, 200));
	
	QPainterPath bigPath;
	if (m_percentStyle == PercentStyle_Rect) {
		width = width - m_borderWidth * 2;
		height = height - m_borderWidth * 2;
		bigPath.addRect(m_borderWidth, m_borderWidth, width, height);
		//���߿�
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
	
	//��·��,�ô�·����ȥ���������·��,�γ�����Ч��
	QPainterPath path;
	painter->setPen(Qt::NoPen);
	QColor waterColor1 = m_frontColor;
	waterColor1.setAlpha(100);
	QColor waterColor2 = m_frontColor;
	waterColor2.setAlpha(180);
	
	//ʹ�ý���ɫ
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
	
	//��һ��������ȥ���·��
	path = bigPath.intersected(waterPath1);
	painter->setBrush(QBrush(linearGradient1));
	painter->drawPath(path);
	
	//�ڶ���������ȥ���·��
	path = bigPath.intersected(waterPath2);
	painter->setBrush(QBrush(linearGradient2));
	painter->drawPath(path);
	painter->restore();
	
	//��������
	m_font.setPixelSize(this->width() / 8);
	painter->setPen(m_textColor);
	painter->setFont(m_font);
	painter->drawText(this->rect(), Qt::AlignCenter, QString("%0%").arg(QString::number(m_value)));
}
