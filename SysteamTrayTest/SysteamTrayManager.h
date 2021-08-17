#pragma once

#include <QObject>
#include <QMenu>
#include <QSystemTrayIcon>

class QAction;
class SystemTrayWarn;

class SysteamTrayManager : public QObject
{
	Q_OBJECT
public:
	static SysteamTrayManager& getInstance();
	~SysteamTrayManager();

	//初始化系统托盘选项
	void initSysteamTray();
	//清理系统托盘
	void clearTray();
	//在系统托盘上设置数据
	void setUserInfoOnTray();

	//打开二次弹窗
	void openSystemTrayWarn();

private slots:
	void sltTrayExit();
	void sltTrayActivated(QSystemTrayIcon::ActivationReason reason);

signals:
	void sigTrayToOpen();
	void sigMinToTray();

private:
	//系统托盘对象
	QSystemTrayIcon m_tray;
	//托盘图标右键菜单
	QMenu m_menuTray;
	//托盘选项
	//打开主界面
	QAction* m_pActionOpen = nullptr;
	//退出
	QAction* m_pActionExit = nullptr;
	//TODO

	//二次提醒弹窗
	QSharedPointer<SystemTrayWarn> m_systemTrayWarn = nullptr;

private:
	SysteamTrayManager(QObject *parent = Q_NULLPTR);
	Q_DISABLE_COPY(SysteamTrayManager)
};

#define systeamTrayManager SysteamTrayManager::getInstance()