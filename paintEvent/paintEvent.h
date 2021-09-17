#pragma once

#include <QtWidgets/QWidget>
#include "ui_paintEvent.h"

class QPainter;
class QVariantAnimation;
class QPropertyAnimation;

class paintEvents : public QWidget
{
    Q_OBJECT

public:
	paintEvents(QWidget *parent = Q_NULLPTR);

private slots:
	void sltAtIndexChanged(QVariant value);
	void sltValueChanged(QVariant value);

private:
	virtual void paintEvent(QPaintEvent *event);

	void paintHeart(QPainter *painter);

	void paintColorText(QPainter *painter);

	void paintAnimationText(QPainter *painter);

	void paintAnimationText2(QPainter *painter);

private:
    Ui::paintEventClass ui;
	//颜色进度
	double m_nAtIndex;
	//控制颜色进度动画
	QVariantAnimation *m_vAnimation = Q_NULLPTR;
	//透明度
	int m_nAlpha;
	//动画类
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
};
