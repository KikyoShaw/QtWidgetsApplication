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
	QString userId;      //用户唯一id
	QString logoPath;      //头像
	QString nickName;       //昵称
	QString text;          //个签
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
	//根据窗口大小改变每项聊天内容的高度
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
	//用户信息
	userDataInfo m_userInfo;
	//窗口管理
	QObjectCleanupHandler m_objectCleaner;
	//调整控件大小
	QList<QPointer<ChatMessageBrowser>> m_chatbrowsers;
	//聊天消息布局
	QVBoxLayout *m_layoutMessages = Q_NULLPTR;
	//时间戳
	quint64 m_timeStamp;
};

