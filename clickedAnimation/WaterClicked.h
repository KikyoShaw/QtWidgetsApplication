#pragma once

#include <QtWidgets/QWidget>
#include <QVariantAnimation>

class WaterDrop : public QWidget
{
	Q_OBJECT

public:
	WaterDrop(QWidget *parent = Q_NULLPTR);
	~WaterDrop();

	void show();
	//设置显示位置
	void move(const QPoint &point);
	// 设置水滴的颜色, 默认为黑白
	void setColor(QColor color);

private:
	// 绘制鼠标的水滴点击效果
	void paintEvent(QPaintEvent *event);

public slots:
	void sltRaduisChanged(QVariant value);

private:
	QVariantAnimation* m_waterDropAnimation = nullptr;
	// 水滴变化的半径;
	int m_animationRadius;
	// 水滴的颜色;
	QColor m_waterDropColor;
};
