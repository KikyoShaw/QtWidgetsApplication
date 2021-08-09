#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QObjectCleanupHandler>
#include <QPointer>
#include "ui_chatMessageItem.h"

class ChatMessageBrowser;

class userDataInfo
{
public:
	QString userId;      //�û�Ψһid
	QString logoPath;      //ͷ��
	QString nickName;       //�ǳ�
	QString text;          //��ǩ
public:
	void clear() {
		logoPath.clear();
		nickName.clear();
		text.clear();
	}
};

class ChatMessageItem :public QWidget
{
	Q_OBJECT

public:
	explicit ChatMessageItem(QWidget *parent = nullptr);
	~ChatMessageItem();

	void setUserDataInfo(const userDataInfo&);

private:
	void initUI();
	void clearUI();
	void sendMessage(const QString & text, bool isMyself);
	void addTimeLabel();
	//���ݴ��ڴ�С�ı�ÿ���������ݵĸ߶�
	void resizeControls();

signals:
	void sigMinimized();
	void sigMaxOrNomal();
	void sigClose();
	void sigDeleteItem(const QString &userId);

private slots:
	void sltSendMessage();

protected:
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	Ui::qChatItem ui;
	//�û���Ϣ
	userDataInfo m_userInfo;
	//���ڹ���
	QObjectCleanupHandler m_objectCleaner;
	//�����ؼ���С
	QList<QPointer<ChatMessageBrowser>> m_chatbrowsers;
	//������Ϣ����
	QVBoxLayout *m_layoutMessages = Q_NULLPTR;
	//ʱ���
	quint64 m_timeStamp;
};

