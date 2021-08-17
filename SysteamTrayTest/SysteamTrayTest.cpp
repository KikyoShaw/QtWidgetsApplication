#include "SysteamTrayTest.h"
#include <QMouseEvent>
#include "SysteamTrayManager.h"
#include <QSettings>
#include "SystemTrayWarn.h"

SysteamTrayTest::SysteamTrayTest(QWidget *parent)
    : QWidget(parent),m_point(QPoint()), m_isMove(false)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	systeamTrayManager.initSysteamTray();
	systeamTrayManager.setUserInfoOnTray();

	connect(ui.pushButton, &QPushButton::clicked, this, &SysteamTrayTest::sltExitOrMinToTray);
	//系统托盘打开总控台
	connect(&systeamTrayManager, &SysteamTrayManager::sigTrayToOpen, this, &QWidget::show);
	//最小化
	connect(&systeamTrayManager, &SysteamTrayManager::sigMinToTray, this, &QWidget::hide);
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
	//读取配置文件
//创建配置文件操作对象,实时读取是否退出
	QSettings settings(Common_Setting_FileName, QSettings::IniFormat);
	//读取是否退出还是缩小到托盘
	QString path = QString("%1/%2").arg("config").arg("status");
	int exits = settings.value(path).toInt();
	return exits;
}
