#pragma once

#include <QObject>
#include <QMenu>
#include <QSystemTrayIcon>

class QSystemTrayIcon;
class QAction;

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

private slots:
	void sltTrayExit();
	void sltTrayActivated(QSystemTrayIcon::ActivationReason reason);

signals:
	void sigTrayToOpen();

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

private:
	SysteamTrayManager(QObject *parent = Q_NULLPTR);
	Q_DISABLE_COPY(SysteamTrayManager)
};

#define systeamTrayManager SysteamTrayManager::getInstance()