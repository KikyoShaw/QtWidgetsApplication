#pragma once

#include <QtWidgets/QWidget>
#include "ui_SysteamTrayTest.h"

class QTimer;

class SysteamTrayTest : public QWidget
{
    Q_OBJECT

public:
    SysteamTrayTest(QWidget *parent = Q_NULLPTR);
	~SysteamTrayTest();

private slots:
	void sltExitOrMinToTray();
	void sltButtonToggled(bool check);
	void sltTimeout();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	//�˳����0.��������ʾ��1.������С�����̣�2.����ֱ���˳�
	int exitsCheckINI();

private:
    Ui::SysteamTrayTestClass ui;
	QPoint m_point;
	bool m_isMove;

	QTimer *m_timer = nullptr;
};
