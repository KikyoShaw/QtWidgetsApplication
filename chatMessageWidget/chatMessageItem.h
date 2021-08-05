#pragma once

#include <QWidget>
#include <QMouseEvent>
#include "ui_chatMessageItem.h"

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

signals:
	void sigMinimized();
	void sigMaxOrNomal();
	void sigClose();
	void sigDeleteItem(const QString &userId);

private:
	Ui::qChatItem ui;
	QString m_userId;
};

