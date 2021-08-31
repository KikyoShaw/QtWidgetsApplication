#include "birthInfoWidget.h"

BirthInfoWidget::BirthInfoWidget(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent), m_astroList(QStringList()), m_astroDayList(QStringList())
	, m_year(0), m_month(0), m_day(0)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	initAstroVec();
	initRollingBox();

	setBirthDate(1995, 8, 10);
}

BirthInfoWidget::~BirthInfoWidget()
{
}

void BirthInfoWidget::setBirthDate(int year, int month, int day)
{
	//保存当前值
	m_year = year;
	m_month = month;
	m_day = day;
	//设置天数
	setDay(year, month);
	//设置滑动控件
	ui.widget_year->setValue(year);
	ui.widget_month->setValue(month);
	ui.widget_day->setValue(day);
	//设置年龄和星座
	setAstro(month, day);
	setAge(year, month, day);
}

bool BirthInfoWidget::isLoopYear(int year)
{
	return (((0 == (year % 4)) && (0 != (year % 100))) || (0 == (year % 400)));
}

int BirthInfoWidget::getMonthDays(int year, int month)
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

int BirthInfoWidget::getYearDays(int year)
{
	int countDay = 0;
	int loopDay = isLoopYear(year) ? 1 : 0;
	countDay = 365 + loopDay;
	return countDay;
}

void BirthInfoWidget::initAstroVec()
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

QString BirthInfoWidget::getAstro(int month, int day)
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

int BirthInfoWidget::getAge(int year, int month, int day)
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

void BirthInfoWidget::initRollingBox()
{
	//初始化滑动控件
	//年,由于需求选择年到今年需要隔18年，因此需要特殊处理
	//获取当前年份
	QDate currentdate = QDate::currentDate();
	auto currenYear = currentdate.year();
	auto rollingBoxYear = currenYear - 18;
	ui.widget_year->setRang(1979, rollingBoxYear);
	ui.widget_year->setPropertys(E_Property_Year);
	//月
	ui.widget_month->setRang(1, 12);
	ui.widget_month->setPropertys(E_Property_Month);
	//日
	ui.widget_day->setPropertys(E_Property_Day);
	//信号与槽
	connect(ui.widget_year, &RollingBox::sigCurrentValueChange, this, &BirthInfoWidget::sltCurrentValueChange);
	connect(ui.widget_month, &RollingBox::sigCurrentValueChange, this, &BirthInfoWidget::sltCurrentValueChange);
	connect(ui.widget_day, &RollingBox::sigCurrentValueChange, this, &BirthInfoWidget::sltCurrentValueChange);
}

void BirthInfoWidget::setDay(int year, int month)
{
	int day = getMonthDays(year, month);
	ui.widget_day->setRang(1, day);
}

void BirthInfoWidget::setAstro(int month, int day)
{
	auto astro = getAstro(m_month, m_day);
	ui.label_astro->setText(astro);
}

void BirthInfoWidget::setAge(int year, int month, int day)
{
	auto age = getAge(year, month, day);
	ui.label_age->setText(QStringLiteral("%1岁").arg(QString::number(age)));
}

void BirthInfoWidget::sltCurrentValueChange(int nValue, DateProperty dateProperty)
{
	switch (dateProperty)
	{
	case E_Property_None:
		break;
	case E_Property_Day:{
		m_day = nValue;
		setAstro(m_month, m_day);
		setAge(m_year, m_month, m_day);
		break;
	}
	case E_Property_Month: {
		m_month = nValue;
		setDay(m_year, m_month);
		setAstro(m_month, m_day);
		setAge(m_year, m_month, m_day);
		break;
	}
	case E_Property_Year: {
		m_year = nValue;
		setDay(m_year, m_month);
		setAge(m_year, m_month, m_day);
		break;
	}
	default:
		break;
	}
	emit sigDateValueChange(m_year, m_month, m_day);
}
