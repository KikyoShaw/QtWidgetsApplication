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
	//������Ч
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
	//��Ӱ��
	QGraphicsDropShadowEffect *m_shadowEffect = Q_NULLPTR;
	//��Ӱ������Ч
	QVariantAnimation *m_vShadowEffectAnimation = Q_NULLPTR;
	//͸����
	QGraphicsOpacityEffect *m_opacityEffect = Q_NULLPTR;
	//͸��������Ч
	QVariantAnimation *m_vOpacityEffectAnimation = Q_NULLPTR;
	//�����˶�����
	QPropertyAnimation *m_pCurAnimation = Q_NULLPTR;
};
