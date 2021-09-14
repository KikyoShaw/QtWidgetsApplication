#pragma once

#include <QtWidgets/QWidget>
#include "ui_animationTest.h"

class QPropertyAnimation;
class QParallelAnimationGroup;

class animationTest : public QWidget
{
    Q_OBJECT

public:
    explicit animationTest(QWidget *parent = Q_NULLPTR);
	~animationTest();

private:
	void initLabel();
	void initAnimation();
	void changeAnimationValue();

private slots:
	void sltStartAnimation();

private:
    Ui::animationTestClass ui;
	//动画特效
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
	//动画管理
	QParallelAnimationGroup *m_animationGroup = Q_NULLPTR;
	//控件管理
	QVector<QLabel*> m_labelVec;
	//动画类管理
	QVector<QPropertyAnimation *> m_animationVec;
	//随机位置
	int m_posX;
	int m_posY;
};
