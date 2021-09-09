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
	//���浱ǰֵ
	m_year = year;
	m_month = month;
	m_day = day;
	//��������
	setDay(year, month);
	//���û����ؼ�
	ui.widget_year->setValue(year);
	ui.widget_month->setValue(month);
	ui.widget_day->setValue(day);
	//�������������
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
	//��ȡ��ǰ������
	QDate today = QDate::currentDate();
	auto _year = today.year();
	auto _month = today.month();
	auto _day = today.day();

	//��������
	//������򣺱Ƚ����ͬʱȷ���·��ڵ�ǰ�·�֮ǰ�������֮�󣬴�����һ��
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

void BirthInfoWidget::initRollingBox()
{
	//��ʼ�������ؼ�
	//��,��������ѡ���굽������Ҫ��18�꣬�����Ҫ���⴦��
	//��ȡ��ǰ���
	QDate currentdate = QDate::currentDate();
	auto currenYear = currentdate.year();
	auto rollingBoxYear = currenYear - 18;
	ui.widget_year->setRang(1979, rollingBoxYear);
	ui.widget_year->setPropertys(E_Property_Year);
	//��
	ui.widget_month->setRang(1, 12);
	ui.widget_month->setPropertys(E_Property_Month);
	//��
	ui.widget_day->setPropertys(E_Property_Day);
	//�ź����
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
	ui.label_age->setText(QStringLiteral("%1��").arg(QString::number(age)));
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