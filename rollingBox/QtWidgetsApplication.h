#pragma once

#include <QtWidgets/QWidget>
#include "ui_QtWidgetsApplication.h"

class QtWidgetsApplication : public QWidget
{
    Q_OBJECT

public:
    explicit QtWidgetsApplication(QWidget *parent = Q_NULLPTR);
	~QtWidgetsApplication();

private slots:
	void sltGetDate();

	//滑动器选择日期信号与槽
	void sltCurrentValueChange(int nValue, DateProperty dateProperty);

private:
	void initAstroVec();
	void initRollingBox();
	//获取星座
	QString getAstro(int month, int day);
	//获取年龄
	//int getAge(int year);
	//设置星座
	void setAstro(int month, int day);
	//设置年龄
	void setAge(int year, int month, int day);
	
	void setLineEdit();

	//计算是否闰年
	bool isLoopYear(int year);

	//计算指定年月该月共多少天
	int getMonthDays(int year, int month);

	//计算指定年有多少天
	int getYearDays(int year);

	//计算年龄
	int getAge(int year, int month, int day);

	//设置天数
	void setDay(int year, int month);

private:
    Ui::QtWidgetsApplicationClass ui;
	//星座
	QStringList m_astroList;
	//星座之间的分割点
	QStringList m_astroDayList;
	//
	int m_year;
	int m_month;
	int m_day;
};
