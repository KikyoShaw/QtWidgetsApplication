#include "SysteamTrayManager.h"
#include <QAction>
#include <QApplication>

SysteamTrayManager & SysteamTrayManager::getInstance()
{
	static SysteamTrayManager m_instance;
	return m_instance;
}

SysteamTrayManager::SysteamTrayManager(QObject *parent)
	: QObject(parent)
{
}

SysteamTrayManager::~SysteamTrayManager()
{
}

void SysteamTrayManager::initSysteamTray()
{
	//ϵͳ��������
	m_pActionOpen = m_menuTray.addAction(QStringLiteral("��������"));
	m_pActionExit = m_menuTray.addAction(QIcon(":/ic_tc.png"), QStringLiteral("�˳�����"));
	connect(m_pActionOpen, &QAction::triggered, this, &SysteamTrayManager::sigTrayToOpen);
	connect(m_pActionExit, &QAction::triggered, this, &SysteamTrayManager::sltTrayExit);
	m_tray.setIcon(QIcon(":/SysteamTrayTest/qrc/Qt.png"));
	m_tray.setContextMenu(&m_menuTray);
	connect(&m_tray, &QSystemTrayIcon::activated, this, &SysteamTrayManager::sltTrayActivated);
	m_tray.show();
}

void SysteamTrayManager::clearTray()
{
	m_tray.hide();
	m_tray.setContextMenu(nullptr);
	m_menuTray.clear();
}

void SysteamTrayManager::setUserInfoOnTray()
{
	QString tip = QStringLiteral("CSDN��%1\nGithub��%2").arg("https://blog.csdn.net/qq_36651243").arg("https://github.com/KikyoShaw");
	m_tray.setToolTip(tip);
}

void SysteamTrayManager::sltTrayExit()
{
	// �����ͷ���Դ
	qApp->quit();
}

void SysteamTrayManager::sltTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
	// ˫������ͼ�꣬�ʹ�������
	if (QSystemTrayIcon::DoubleClick == reason) {
		emit sigTrayToOpen();
	}
}

