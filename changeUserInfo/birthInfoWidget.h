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
	//�����Ƿ�����
	bool isLoopYear(int year);
	//����ָ�����¸��¹�������
	int getMonthDays(int year, int month);
	//����ָ�����ж�����
	int getYearDays(int year);
	//��ʼ������
	void initAstroVec();
	//��ȡ����
	QString getAstro(int month, int day);
	//��������
	int getAge(int year, int month, int day);
	//��ʼ�������ջ����ؼ�
	void initRollingBox();

	//��������, ����ÿ����������ͬ����Ҫ���⴦��
	void setDay(int year, int month);
	//��������
	void setAstro(int month, int day);
	//��������
	void setAge(int year, int month, int day);

	//������ѡ�������ź����
	void sltCurrentValueChange(int nValue, DateProperty dateProperty);

signals:
	void sigDateValueChange(int year, int month, int day);

private:
	Ui::birthInfo ui;
	//����
	QStringList m_astroList;
	//����֮��ķָ��
	QStringList m_astroDayList;
	//����������
	int m_year;
	int m_month;
	int m_day;
};
