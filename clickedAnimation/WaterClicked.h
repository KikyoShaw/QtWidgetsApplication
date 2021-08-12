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
	//������ʾλ��
	void move(const QPoint &point);
	// ����ˮ�ε���ɫ, Ĭ��Ϊ�ڰ�
	void setColor(QColor color);

private:
	// ��������ˮ�ε��Ч��
	void paintEvent(QPaintEvent *event);

public slots:
	void sltRaduisChanged(QVariant value);

private:
	QVariantAnimation* m_waterDropAnimation = nullptr;
	// ˮ�α仯�İ뾶;
	int m_animationRadius;
	// ˮ�ε���ɫ;
	QColor m_waterDropColor;
};
