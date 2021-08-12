#include "WaterClicked.h"
#include <QPainter>

// ˮ�εİ뾶;
#define WATER_DROP_RADIUS 40

WaterDrop::WaterDrop(QWidget *parent)
	: QWidget(parent)
	, m_animationRadius(0)
	, m_waterDropColor(QColor(0, 0, 0, 15))
{
	setWindowFlags(Qt::FramelessWindowHint /*| Qt::Tool*/);
	setAttribute(Qt::WA_TranslucentBackground);

	setFixedSize(QSize(WATER_DROP_RADIUS * 2, WATER_DROP_RADIUS * 2));

	// �ؼ���ʾ��رպ��Զ�ɾ��
	setAttribute(Qt::WA_DeleteOnClose);

	m_waterDropAnimation = new QVariantAnimation(this);
}

WaterDrop::~WaterDrop()
{
}

void WaterDrop::move(const QPoint &point)
{
	// ����Ҫ��������ĵ�ת��ΪԲ�ĵ�����;
	QPoint translatePoint = point - QPoint(WATER_DROP_RADIUS, WATER_DROP_RADIUS);
	__super::move(translatePoint);
}

void WaterDrop::show()
{
	__super::show();

	// ͨ�������಻�Ͻ����ػ�;
	if (m_waterDropAnimation) {
		m_waterDropAnimation->setStartValue(0);
		m_waterDropAnimation->setEndValue(WATER_DROP_RADIUS);
		m_waterDropAnimation->setDuration(380);

		connect(m_waterDropAnimation, &QVariantAnimation::valueChanged, this, &WaterDrop::sltRaduisChanged);
		connect(m_waterDropAnimation, &QVariantAnimation::finished, this, &WaterDrop::close);
		m_waterDropAnimation->start();
	}

}

void WaterDrop::setColor(QColor color)
{
	m_waterDropColor = color;
}

void WaterDrop::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(m_waterDropColor));

	// ˼·�����Ȼ���һ���̶���С��ԲA��Ȼ�����ͬһԲ�ĵ�͸����ԲB��Ȼ��ͨ����������ԲB�İ뾶��0������WATER_DROP_RADIUS�����¸��ǹ̶���ԲA
	QPainterPath waterDropPath;
	waterDropPath.addEllipse(QPoint(WATER_DROP_RADIUS, WATER_DROP_RADIUS), WATER_DROP_RADIUS, WATER_DROP_RADIUS);
	QPainterPath hidePath;
	hidePath.addEllipse(QPoint(WATER_DROP_RADIUS, WATER_DROP_RADIUS), m_animationRadius, m_animationRadius);

	waterDropPath -= hidePath;
	painter.drawPath(hidePath);
}

void WaterDrop::sltRaduisChanged(QVariant value)
{
	// ��������ԲB�İ뾶ֵ,���ػ�
	m_animationRadius = value.toInt();
	update();
}