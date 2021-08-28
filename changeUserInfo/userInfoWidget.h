#pragma once

#include <QWidget>
#include "ui_userInfoWidget.h"

class UserInfoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit UserInfoWidget(QWidget *parent = Q_NULLPTR);
	~UserInfoWidget();

private:
	Ui::userInfo ui;
};