#include "QtWidgetsApplication.h"
#include <QRegExpValidator>
#include <QDate>

QtWidgetsApplication::QtWidgetsApplication(QWidget *parent)
    : QWidget(parent), m_year(1989), m_month(6), m_day(1)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//������
	QRegExp regx("[0-9]+$");
	QValidator *validator = new QRegExpValidator(regx);
	ui.lineEdit_day->setValidator(validator);
	ui.lineEdit_month->setValidator(validator);
	ui.lineEdit_year->setValidator(validator);

	initAstroVec();
	initRollingBox();

	connect(ui.pushButton_close, &QPushButton::clicked, this, &QtWidgetsApplication::close);
	connect(ui.pushButton_sure, &QPushButton::clicked, this, &QtWidgetsApplication::sltGetDate);
}

QtWidgetsApplication::~QtWidgetsApplication()
{
}

void QtWidgetsApplication::initAstroVec()
{
	/*
	������ Aries (03/21 - 04/19)
	��ţ�� Taurus (04/20 - 05/20)
	˫���� Gemini (05/21 - 06/21)
	��з�� Cancer (06/22 - 07/22)
	ʨ���� Leo (07/23 - 08/22)
	��Ů�� Virgo (08/23 - 09/22)
	����� Libra (09/23 - 10/23)
	��Ы�� Scorpio (10/24 - 11/22)
	������ Sagittarius (11/23 - 12/21)
	Ħ���� Capricorn (12/22 - 01/19)
	ˮƿ�� Aquarius (01/20 - 02/18)
	˫���� Pisces (02/19 - 03/20)
	*/
	QString astro = QStringLiteral("ħ����,ˮƿ��,˫����,������,��ţ��,˫����,��з��,ʨ����,��Ů��,�����,��Ы��,������,Ħ����");
	m_astroList = astro.split(",");
	QString astroDay = QStringLiteral("20,19,21,21,21,22,23,23,23,23,22,22");
	m_astroDayList = astroDay.split(",");
}

void QtWidgetsApplication::initRollingBox()
{
	setAstro();
	setLineEdit();
	setAge();
	//��
	ui.widget_year->setRang(1979, 2002);
	ui.widget_year->setValue(m_year);
	connect(ui.widget_year, &RollingBox::sigCurrentValueChange, this, [=](int nYear) {
		m_year = nYear;
		setDay(m_year, m_month);
		setAge();
		setLineEdit();
	});
	//��
	//
	ui.widget_month->setRang(1, 12);
	ui.widget_month->setValue(m_month);
	connect(ui.widget_month, &RollingBox::sigCurrentValueChange, this, [=](int nMonth) {
		m_month = nMonth;
		setDay(m_year, m_month);
		setAstro();
		setAge();
		setLineEdit();
	});
	//��
	setDay(m_year, m_month);
	ui.widget_day->setValue(m_day);
	connect(ui.widget_day, &RollingBox::sigCurrentValueChange, this, [=](int nDay) {
		m_day = nDay;
		setAstro();
		setAge();
		setLineEdit();
	});
}

QString QtWidgetsApplication::getAstro(int month, int day)
{
	QString astro = QString();
	int astroDayIndex = month - 1;
	int astroIndex = month;
	if (m_astroDayList.size() < astroDayIndex || m_astroList.size() < astroIndex) {
		return astro;
	}
	auto astroDay = m_astroDayList.value(astroDayIndex).toInt();
	if (day < astroDay) {
		astroIndex = astroIndex - 1;
	}
	astro = m_astroList.value(astroIndex);
	return astro;
}

void QtWidgetsApplication::setAstro()
{
	auto astro = getAstro(m_month, m_day);
	ui.label_constellation->setText(QStringLiteral("������%1").arg(astro));
}

void QtWidgetsApplication::setAge()
{
	/*QDate birthday(m_year, m_month, m_day);
	QDate today = QDate::currentDate();
	int daysold = birthday.daysTo(today);
	int age = daysold / 365;*/
	int age = getAge(m_year, m_month, m_day);
	ui.label_age->setText(QStringLiteral("���䣺%1").arg(QString::number(age)));
}

void QtWidgetsApplication::setLineEdit()
{
	ui.lineEdit_day->setText(QString::number(m_day));
	ui.lineEdit_month->setText(QString::number(m_month));
	ui.lineEdit_year->setText(QString::number(m_year));
}

bool QtWidgetsApplication::isLoopYear(int year)
{
	return (((0 == (year % 4)) && (0 != (year % 100))) || (0 == (year % 400)));
}

int QtWidgetsApplication::getMonthDays(int year, int month)
{
	int countDay = 0;
	int loopDay = isLoopYear(year) ? 1 : 0;

	switch (month) {
	case 1:
		countDay = 31;
		break;
	case 2:
		countDay = 28 + loopDay;
		break;
	case 3:
		countDay = 31;
		break;
	case 4:
		countDay = 30;
		break;
	case 5:
		countDay = 31;
		break;
	case 6:
		countDay = 30;
		break;
	case 7:
		countDay = 31;
		break;
	case 8:
		countDay = 31;
		break;
	case 9:
		countDay = 30;
		break;
	case 10:
		countDay = 31;
		break;
	case 11:
		countDay = 30;
		break;
	case 12:
		countDay = 31;
		break;
	default:
		countDay = 30;
		break;
	}

	return countDay;
}

int QtWidgetsApplication::getYearDays(int year)
{
	int countDay = 0;
	int loopDay = isLoopYear(year) ? 1 : 0;
	countDay = 365 + loopDay;
	return countDay;
}

int QtWidgetsApplication::getAge(int year, int month, int day)
{
	//��ȡ��ǰ������
	QDate today = QDate::currentDate();
	auto _year = today.year();
	auto _month = today.month();
	auto _day = today.day();

	//��������
	//������򣺱Ƚ����ͬʱȷ���·��ڵ�ǰ�·�֮ǰ�������֮�󣬴����һ��
	//�·���ͬ������Ҫ����������������֮��ͬ����Ҫ��һ��
	int age = _year - year;
	if (_month < month) {
		age += 1;
	}
	else if (_month == month) {
		if (_day < day) {
			age += 1;
		}
	}
	return age;
}

void QtWidgetsApplication::setDay(int year, int month)
{
	int day = getMonthDays(year, month);
	ui.widget_day->setRang(1, day);
}

void QtWidgetsApplication::sltGetDate()
{
	m_day = ui.lineEdit_day->text().toInt();
	m_month = ui.lineEdit_month->text().toInt();
	m_year = ui.lineEdit_year->text().toInt();

	setAge();
	setAstro();
}
