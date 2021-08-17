#include "SysteamTrayTest.h"
#include <QMouseEvent>
#include "SysteamTrayManager.h"
#include <QSettings>
#include <QTimer>
#include "SystemTrayWarn.h"
#include <QDebug>

constexpr char* Property_timerId = "timerId";

SysteamTrayTest::SysteamTrayTest(QWidget *parent)
    : QWidget(parent),m_point(QPoint()), m_isMove(false)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	//��ʼ��������Ϣ
	systeamTrayManager.initSysteamTray();
	systeamTrayManager.setUserInfoOnTray();

	//��ʼ����ʱ��
	m_timer = new QTimer(this);
	if (m_timer) {
		m_timer->setInterval(300);
		m_timer->setProperty(Property_timerId, true);
		connect(m_timer, &QTimer::timeout, this, &SysteamTrayTest::sltTimeout);
	}

	//��ʼ��������
	m_variantAnimation = new QVariantAnimation(this);
	if (m_variantAnimation) {
		m_variantAnimation->setStartValue(0);
		m_variantAnimation->setEndValue(2);
		m_variantAnimation->setLoopCount(-1);
		m_variantAnimation->setDuration(500);
		connect(m_variantAnimation, &QVariantAnimation::valueChanged, this, &SysteamTrayTest::sltVariantChanged);
	}

	connect(ui.pushButton, &QPushButton::clicked, this, &SysteamTrayTest::sltExitOrMinToTray);
	//ϵͳ���̴��ܿ�̨
	connect(&systeamTrayManager, &SysteamTrayManager::sigTrayToOpen, this, &QWidget::show);
	//��С��
	connect(&systeamTrayManager, &SysteamTrayManager::sigMinToTray, this, &QWidget::hide);

	//ϵͳ������˸
	connect(ui.pushButton_test, &QPushButton::toggled, this, &SysteamTrayTest::sltButtonToggled);
}

SysteamTrayTest::~SysteamTrayTest()
{
}

void SysteamTrayTest::sltExitOrMinToTray()
{
	int code = exitsCheckINI();
	if (ExitStatus::E_Tray == code) {
		hide();
	}
	else if (ExitStatus::E_Exit == code) {
		qApp->quit();
	}
	else {
		systeamTrayManager.openSystemTrayWarn();
	}
}

void SysteamTrayTest::sltButtonToggled(bool check)
{
	if (check) {
		/*if (m_timer) {
			m_timer->start();
		}*/
		qInfo() << "111";
		if (m_variantAnimation) {
			if (QAbstractAnimation::Running != m_variantAnimation->state()) {
				m_variantAnimation->start();
			}
		}
	}
	else {
		/*if (m_timer) {
			m_timer->stop();
		}*/
		if (m_variantAnimation) {
			if (QAbstractAnimation::Stopped != m_variantAnimation->state()) {
				m_variantAnimation->stop();
			}
		}
		systeamTrayManager.changeTrayIcon(":/SysteamTrayTest/qrc/Qt.png");
	}
}

void SysteamTrayTest::sltTimeout()
{
	bool result = m_timer->property(Property_timerId).toBool();
	m_timer->setProperty(Property_timerId, !result);
	if (result) {
		systeamTrayManager.changeTrayIcon(":/SysteamTrayTest/qrc/1.png");
	}
	else {
		systeamTrayManager.changeTrayIcon(":/SysteamTrayTest/qrc/Qt.png");
	}
}

void SysteamTrayTest::sltVariantChanged(QVariant value)
{
	auto result = value.toInt();
	qInfo() << result;
	if (0 == result) {
		systeamTrayManager.changeTrayIcon(":/SysteamTrayTest/qrc/1.png");
	}
	else {
		systeamTrayManager.changeTrayIcon(":/SysteamTrayTest/qrc/Qt.png");
	}
}

void SysteamTrayTest::mouseMoveEvent(QMouseEvent * event)
{
	if ((event->buttons() & Qt::LeftButton) && m_isMove) {
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void SysteamTrayTest::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		m_isMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void SysteamTrayTest::mouseReleaseEvent(QMouseEvent * event)
{
	m_isMove = false;
	QWidget::mouseReleaseEvent(event);
}

int SysteamTrayTest::exitsCheckINI()
{
	//��ȡ�����ļ�
//���������ļ���������,ʵʱ��ȡ�Ƿ��˳�
	QSettings settings(Common_Setting_FileName, QSettings::IniFormat);
	//��ȡ�Ƿ��˳�������С������
	QString path = QString("%1/%2").arg("config").arg("status");
	int exits = settings.value(path).toInt();
	return exits;
}
