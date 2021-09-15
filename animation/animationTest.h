#pragma once

#include <QtWidgets/QWidget>
#include "ui_animationTest.h"

class QPropertyAnimation;
class QGraphicsDropShadowEffect;
class QVariantAnimation;
class QGraphicsOpacityEffect;

class animationTest : public QWidget
{
    Q_OBJECT

public:
    explicit animationTest(QWidget *parent = Q_NULLPTR);
	~animationTest();

private:
	void initShadowEffect();
	void initCurvilinear();

private slots:
	void sltStartPopAnimation();
	void sltStartHideAnimation();
	void sltStartOpacityAnimation();
	void sltStartShadowEffectAnimation();
	void sltShadowEffectRaduisChanged(QVariant value);
	void sltStartOpacityEffectAnimation();
	void sltStartCurAnimation();
	void sltMoveChanged();

private:
    Ui::animationTestClass ui;
	//动画特效
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
	//阴影类
	QGraphicsDropShadowEffect *m_shadowEffect = Q_NULLPTR;
	//阴影控制特效
	QVariantAnimation *m_vShadowEffectAnimation = Q_NULLPTR;
	//透明类
	QGraphicsOpacityEffect *m_opacityEffect = Q_NULLPTR;
	//透明控制特效
	QVariantAnimation *m_vOpacityEffectAnimation = Q_NULLPTR;
	//曲线运动控制
	QPropertyAnimation *m_pCurAnimation = Q_NULLPTR;
};
