#pragma once

#include <QtWidgets/QWidget>
#include "ui_animationTest.h"

class QPropertyAnimation;
class QGraphicsDropShadowEffect;

class animationTest : public QWidget
{
    Q_OBJECT

public:
    explicit animationTest(QWidget *parent = Q_NULLPTR);
	~animationTest();

private slots:
	void sltStartPopAnimation();
	void sltStartHideAnimation();
	void sltStartOpacityAnimation();
	void sltStartShadowEffectAnimation();
	void sltShadowEffectRaduisChanged(QVariant value);
	void sltMoveChanged();

private:
    Ui::animationTestClass ui;
	//������Ч
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
	//��Ӱ��
	QGraphicsDropShadowEffect *m_shadowEffect = Q_NULLPTR;
	//��Ӱ������Ч
	QVariantAnimation *m_vShadowEffectAnimation = Q_NULLPTR;
};
