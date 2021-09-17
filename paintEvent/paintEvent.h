#pragma once

#include <QtWidgets/QWidget>
#include "ui_paintEvent.h"

class QPainter;
class QVariantAnimation;

class paintEvents : public QWidget
{
    Q_OBJECT

public:
	paintEvents(QWidget *parent = Q_NULLPTR);

private slots:
	void sltAtIndexChanged(QVariant value);

private:
	virtual void paintEvent(QPaintEvent *event);

	void paintHeart(QPainter *painter);

	void paintColorText(QPainter *painter);


private:
    Ui::paintEventClass ui;
	//��ɫ����
	double m_nAtIndex;
	//������ɫ���ȶ���
	QVariantAnimation *m_vAnimation = Q_NULLPTR;
};
