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

	//��ʼ��ϵͳ����ѡ��
	void initSysteamTray();
	//����ϵͳ����
	void clearTray();
	//��ϵͳ��������������
	void setUserInfoOnTray();

	//�򿪶��ε���
	void openSystemTrayWarn();

private slots:
	void sltTrayExit();
	void sltTrayActivated(QSystemTrayIcon::ActivationReason reason);

signals:
	void sigTrayToOpen();
	void sigMinToTray();

private:
	//ϵͳ���̶���
	QSystemTrayIcon m_tray;
	//����ͼ���Ҽ��˵�
	QMenu m_menuTray;
	//����ѡ��
	//��������
	QAction* m_pActionOpen = nullptr;
	//�˳�
	QAction* m_pActionExit = nullptr;
	//TODO

	//�������ѵ���
	QSharedPointer<SystemTrayWarn> m_systemTrayWarn = nullptr;

private:
	SysteamTrayManager(QObject *parent = Q_NULLPTR);
	Q_DISABLE_COPY(SysteamTrayManager)
};

#define systeamTrayManager SysteamTrayManager::getInstance()