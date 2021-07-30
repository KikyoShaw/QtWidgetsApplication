#include "QtWidgetsApplication.h"
#include <QRegExpValidator>
#include <QDate>

QtWidgetsApplication::QtWidgetsApplication(QWidget *parent)
    : QWidget(parent), m_year(1989), m_month(6), m_day(15)
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
		setAge();
		setLineEdit();
	});
	//��
	//
	ui.widget_month->setRang(1, 12);
	ui.widget_month->setValue(m_month);
	connect(ui.widget_month, &RollingBox::sigCurrentValueChange, this, [=](int nMonth) {
		m_month = nMonth;
		setAstro();
		setLineEdit();
	});
	//��
	ui.widget_day->setRang(1, 30);
	ui.widget_day->setValue(m_day);
	connect(ui.widget_day, &RollingBox::sigCurrentValueChange, this, [=](int nDay) {
		m_day = nDay;
		setAstro();
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
	QDate birthday(m_year, m_month, m_day);
	QDate today = QDate::currentDate();
	int daysold = birthday.daysTo(today);
	int age = daysold / 365;
	ui.label_age->setText(QStringLiteral("���䣺%1").arg(QString::number(age)));
}

void QtWidgetsApplication::setLineEdit()
{
	ui.lineEdit_day->setText(QString::number(m_day));
	ui.lineEdit_month->setText(QString::number(m_month));
	ui.lineEdit_year->setText(QString::number(m_year));
}

void QtWidgetsApplication::sltGetDate()
{
	m_day = ui.lineEdit_day->text().toInt();
	m_month = ui.lineEdit_month->text().toInt();
	m_year = ui.lineEdit_year->text().toInt();

	setAge();
	setAstro();
}
