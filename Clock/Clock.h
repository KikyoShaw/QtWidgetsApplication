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
	//计算是否闰年
	bool isLoopYear(int year);

private:
	virtual void paintEvent(QPaintEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

private:
    Ui::ClockClass ui;
	//数据存储
	QStringList strListDate, strListState, strListWeek;
	//定时器
	QTimer* m_time;
};
