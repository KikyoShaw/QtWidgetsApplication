#pragma once

#include <QtWidgets/QWidget>
#include "ui_animationTest.h"

class QPropertyAnimation;

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

private:
    Ui::animationTestClass ui;
	//动画特效
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
	//窗口关闭动画特效
	QPropertyAnimation *m_pOpacityAnimation = Q_NULLPTR;
};
