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

	//������ѡ�������ź����
	void sltCurrentValueChange(int nValue, DateProperty dateProperty);

private:
	void initAstroVec();
	void initRollingBox();
	//��ȡ����
	QString getAstro(int month, int day);
	//��ȡ����
	//int getAge(int year);
	//��������
	void setAstro(int month, int day);
	//��������
	void setAge(int year, int month, int day);
	
	void setLineEdit();

	//�����Ƿ�����
	bool isLoopYear(int year);

	//����ָ�����¸��¹�������
	int getMonthDays(int year, int month);

	//����ָ�����ж�����
	int getYearDays(int year);

	//��������
	int getAge(int year, int month, int day);

	//��������
	void setDay(int year, int month);

private:
    Ui::QtWidgetsApplicationClass ui;
	//����
	QStringList m_astroList;
	//����֮��ķָ��
	QStringList m_astroDayList;
	//
	int m_year;
	int m_month;
	int m_day;
};
