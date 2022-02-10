#pragma once

#include <QtWidgets/QWidget>
#include "ui_Clock.h"

class QTimer;

class Clock : public QWidget
{
    Q_OBJECT

public:
    Clock(QWidget *parent = Q_NULLPTR);
	~Clock();

private:
	void initData();
	double getMaxDate(int y, int m);
	int getWeek(QString str, QStringList list);
	QString getYear(int y);
	//�����Ƿ�����
	bool isLoopYear(int year);

private:
	virtual void paintEvent(QPaintEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

private:
    Ui::ClockClass ui;
	//���ݴ洢
	QStringList strListDate, strListState, strListWeek;
	//��ʱ��
	QTimer* m_time;
};
