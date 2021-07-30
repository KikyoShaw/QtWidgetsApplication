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
	//��ȡ����
	QString getAstro(int month, int day);
	//��ȡ����
	//int getAge(int year);
	//��������
	void setAstro();
	//��������
	void setAge();
	
	void setLineEdit();

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
