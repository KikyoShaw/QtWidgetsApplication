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

private:
	void initAstroVec();
	void initRollingBox();
	//获取星座
	QString getAstro(int month, int day);
	//获取年龄
	//int getAge(int year);
	//设置星座
	void setAstro();
	//设置年龄
	void setAge();
	
	void setLineEdit();

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
