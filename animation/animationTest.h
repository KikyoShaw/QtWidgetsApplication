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
	void sltStartAnimation();

private:
    Ui::animationTestClass ui;
	//¶¯»­ÌØÐ§
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
};
