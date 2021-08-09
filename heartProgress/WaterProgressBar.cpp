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
	//���ö�ʱ���̶�ʱ����ˢ��ҳ�棬ʹ��ˮ�˶�����
	m_timer = new QTimer(this);
	m_timer->setSingleShot(false);
	connect(m_timer, &QTimer::timeout, this, [=]() {
		if (this->isVisible())
		{
			//ƫ�������ƣ�ÿ�λ����Լ�0.3��������һ����������2������ʱ���ͻ���Ϊ0�����϶�ʱ��ÿ50ms����һ�£��ʹ��������ߵĶ�̬Ч��
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
	//���������ɼ�������¾Ͳ��ػ���
	if (!this->isVisible())
	{
		return;
	}
	m_iValue = this->value() < 0 ? 0 : this->value();//��ʼ��ʱQT�������Ľ���Ϊ-1�����⸺���ȵĳ���

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);//����ͼ�����忹���

	drawBackGround(&painter);//���Ʊ���
	drawWaterWave(&painter);//����ˮ��
	drawText(&painter);//���ƽ����ı�
}

void WaterProgressBar::drawBackGround(QPainter* painter)
{
	int width = this->width();
	int height = this->height();
	if (m_iBorderWidth > 0)
	{
		//���ݴ��ڵĳ�����Сֵ�õ��ⲿ����ֱ��
		int max_diameter = qMin(width, height);
		painter->save();
		painter->setBrush(QBrush(m_borderColor));
		painter->setPen(Qt::NoPen);
		//���Ʊ߿򱳾���Ȼ�����ڲ��������ǣ����ɵõ��߿�
		painter->drawEllipse(0, 0, max_diameter, max_diameter);
		painter->restore();
	}
	painter->save();
	//���ݴ��ڵĳ�����Сֵ��ȥ�߿��ȵõ��ڲ�����ֱ��
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
	//���ݴ��ڵĳ�����Сֵ��ȥ�߿��ȵõ�ֱ��
	int diameter = qMin(width, height) - (2 * m_iBorderWidth);

	//sincos���ߵĲ��壬����Խ��ˮ��Խ��
	double waveHeight = 0.04*diameter;
	//sincos���ߵ����ڣ�����Խ��ˮ��Խ��
	double cycle = 2 * M_PI / diameter;
	//ˮ�ĸ߶ȣ��������Ϊ���ȣ�ע���������ǵĵ�������ϵ�������Ͻ�Ϊԭ�㣬������Ǹ�������ϵ���Ĳ�ͬ
	double percent = (double)m_iValue / 100;
	double waterHeight = (1 - percent)*diameter + m_iBorderWidth;

	painter->save();
	QPainterPath totalPath;
	//����Բ��·��
	totalPath.addEllipse(m_iBorderWidth, m_iBorderWidth, diameter, diameter);

	//ˮ��·��,�Ȼ�ǳɫ���ٻ���ɫ
	QPainterPath water1;
	QPainterPath water2;

	water1.moveTo(m_iBorderWidth, m_iBorderWidth + diameter);
	water2.moveTo(m_iBorderWidth, m_iBorderWidth + diameter);

	//�������ʼ�㵽�ұ߽��������������������
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
			//��һ������Y��
			waterY1 = (double)(waveHeight * qSin(cycle * (i - m_iBorderWidth) - M_PI / 2 + m_dOffset)) + waterHeight;//����������ǰ����/2��sin�Ĳ����cos�Ĳ��ȾͶ�����
			//�ڶ�������Y��
			waterY2 = (double)(waveHeight * qCos(cycle * (i - m_iBorderWidth) + m_dOffset)) + waterHeight;
		}
		water1.lineTo(i, waterY1);
		water2.lineTo(i, waterY2);

	}

	//���
	water1.lineTo(m_iBorderWidth + diameter, m_iBorderWidth + diameter);
	water2.lineTo(m_iBorderWidth + diameter, m_iBorderWidth + diameter);

	QPainterPath path;
	QColor waterColor1 = m_waterColor;
	waterColor1.setAlpha(100);
	QColor waterColor2 = m_waterColor;
	waterColor2.setAlpha(200);

	//��һ������
	path = totalPath.intersected(water1);
	painter->setBrush(waterColor1);
	painter->setPen(Qt::NoPen);
	painter->drawPath(path);
	painter->restore();
	painter->save();

	//�ڶ���������ȥ���·��
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
	//���ݴ��ڵĳ�����Сֵ��ȥ�߿��ȵõ�ֱ��
	int diameter = qMin(width, height) - (2 * m_iBorderWidth);
	int fontSize = diameter / 5;
	//�����ı�����
	QFont font;
	font.setFamily("΢���ź�");
	font.setPixelSize(fontSize);
	font.setBold(true);
	//�����ı�
	painter->setFont(font);
	painter->setPen(m_textColor);
	painter->drawText(QRectF(m_iBorderWidth, m_iBorderWidth, diameter, diameter), Qt::AlignCenter, QString("%1%").arg(m_iValue));
	painter->restore();
}