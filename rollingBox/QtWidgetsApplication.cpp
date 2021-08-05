#include "QtWidgetsApplication.h"
#include <QRegExpValidator>
#include <QDate>

QtWidgetsApplication::QtWidgetsApplication(QWidget *parent)
    : QWidget(parent), m_year(1989), m_month(6), m_day(1)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//限制器
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
	白羊座 Aries (03/21 - 04/19)
	金牛座 Taurus (04/20 - 05/20)
	双子座 Gemini (05/21 - 06/21)
	巨蟹座 Cancer (06/22 - 07/22)
	狮子座 Leo (07/23 - 08/22)
	处女座 Virgo (08/23 - 09/22)
	天秤座 Libra (09/23 - 10/23)
	天蝎座 Scorpio (10/24 - 11/22)
	人马座 Sagittarius (11/23 - 12/21)
	摩羯座 Capricorn (12/22 - 01/19)
	水瓶座 Aquarius (01/20 - 02/18)
	双鱼座 Pisces (02/19 - 03/20)
	*/
	QString astro = QStringLiteral("魔羯座,水瓶座,双鱼座,白羊座,金牛座,双子座,巨蟹座,狮子座,处女座,天秤座,天蝎座,射手座,摩羯座");
	m_astroList = astro.split(",");
	QString astroDay = QStringLiteral("20,19,21,21,21,22,23,23,23,23,22,22");
	m_astroDayList = astroDay.split(",");
}

void QtWidgetsApplication::initRollingBox()
{
	setAstro();
	setLineEdit();
	setAge();
	//年
	ui.widget_year->setRang(1979, 2002);
	ui.widget_year->setValue(m_year);
	connect(ui.widget_year, &RollingBox::sigCurrentValueChange, this, [=](int nYear) {
		m_year = nYear;
		setDay(m_year, m_month);
		setAge();
		setLineEdit();
	});
	//月
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
	//日
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
	ui.label_constellation->setText(QStringLiteral("星座：%1").arg(astro));
}

void QtWidgetsApplication::setAge()
{
	/*QDate birthday(m_year, m_month, m_day);
	QDate today = QDate::currentDate();
	int daysold = birthday.daysTo(today);
	int age = daysold / 365;*/
	int age = getAge(m_year, m_month, m_day);
	ui.label_age->setText(QStringLiteral("年龄：%1").arg(QString::number(age)));
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
	//获取当前年月日
	QDate today = QDate::currentDate();
	auto _year = today.year();
	auto _month = today.month();
	auto _day = today.day();

	//计算年龄
	//计算规则：比较年的同时确定月份在当前月份之前，如果在之后，代表加一岁
	//月份相同，则需要计算天数，天数在之后，同样需要加一岁
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
