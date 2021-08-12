#include "WaterClicked.h"
#include <QPainter>

// 水滴的半径;
#define WATER_DROP_RADIUS 40

WaterDrop::WaterDrop(QWidget *parent)
	: QWidget(parent)
	, m_animationRadius(0)
	, m_waterDropColor(QColor(0, 0, 0, 15))
{
	setWindowFlags(Qt::FramelessWindowHint /*| Qt::Tool*/);
	setAttribute(Qt::WA_TranslucentBackground);

	setFixedSize(QSize(WATER_DROP_RADIUS * 2, WATER_DROP_RADIUS * 2));

	// 控件显示完关闭后自动删除
	setAttribute(Qt::WA_DeleteOnClose);

	m_waterDropAnimation = new QVariantAnimation(this);
}

WaterDrop::~WaterDrop()
{
}

void WaterDrop::move(const QPoint &point)
{
	// 这里要把鼠标点击的点转换为圆心点坐标;
	QPoint translatePoint = point - QPoint(WATER_DROP_RADIUS, WATER_DROP_RADIUS);
	__super::move(translatePoint);
}

void WaterDrop::show()
{
	__super::show();

	// 通过动画类不断进行重绘;
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

	// 思路就是先绘制一个固定大小的圆A，然后绘制同一圆心的透明的圆B，然后通过动画类是圆B的半径从0增长到WATER_DROP_RADIUS，以致覆盖固定的圆A
	QPainterPath waterDropPath;
	waterDropPath.addEllipse(QPoint(WATER_DROP_RADIUS, WATER_DROP_RADIUS), WATER_DROP_RADIUS, WATER_DROP_RADIUS);
	QPainterPath hidePath;
	hidePath.addEllipse(QPoint(WATER_DROP_RADIUS, WATER_DROP_RADIUS), m_animationRadius, m_animationRadius);

	waterDropPath -= hidePath;
	painter.drawPath(hidePath);
}

void WaterDrop::sltRaduisChanged(QVariant value)
{
	// 不断增加圆B的半径值,并重绘
	m_animationRadius = value.toInt();
	update();
}