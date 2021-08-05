#pragma once

#include <QWidget>
#include <QHash>
#include "chatMessageWidget.h"

class QPushButton;

class testWidget : public QWidget
{
	Q_OBJECT
public:
	testWidget(QWidget *parent = Q_NULLPTR);
	~testWidget();

private:
	QHash<int, userDataInfo> m_userDataInfo;
	chatMessageWidget m_chat;
	int dataNum;
	QPushButton *m_test = nullptr;
};
