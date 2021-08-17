#pragma once

#include <QtWidgets/QWidget>
#include "ui_SysteamTrayTest.h"

class SysteamTrayTest : public QWidget
{
    Q_OBJECT

public:
    SysteamTrayTest(QWidget *parent = Q_NULLPTR);
	~SysteamTrayTest();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::SysteamTrayTestClass ui;
	QPoint m_point;
	bool m_isMove;
};
