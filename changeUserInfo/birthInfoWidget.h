#pragma once

#include <QWidget>
#include "ui_birthInfoWidget.h"

class BirthInfoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit BirthInfoWidget(QWidget *parent = Q_NULLPTR);
	~BirthInfoWidget();

	void setBirthDate(int year, int month, int day);

private:
	//计算是否闰年
	bool isLoopYear(int year);
	//计算指定年月该月共多少天
	int getMonthDays(int year, int month);
	//计算指定年有多少天
	int getYearDays(int year);
	//初始化星座
	void initAstroVec();
	//获取星座
	QString getAstro(int month, int day);
	//计算年龄
	int getAge(int year, int month, int day);
	//初始化年月日滑动控件
	void initRollingBox();

	//设置天数, 由于每个月天数不同，需要特殊处理
	void setDay(int year, int month);
	//设置星座
	void setAstro(int month, int day);
	//设置年龄
	void setAge(int year, int month, int day);

	//滑动器选择日期信号与槽
	void sltCurrentValueChange(int nValue, DateProperty dateProperty);

signals:
	void sigDateValueChange(int year, int month, int day);

private:
	Ui::birthInfo ui;
	//星座
	QStringList m_astroList;
	//星座之间的分割点
	QStringList m_astroDayList;
	//出生年月日
	int m_year;
	int m_month;
	int m_day;
};
