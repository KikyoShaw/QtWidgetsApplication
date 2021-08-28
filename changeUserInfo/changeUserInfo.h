#pragma once

#include <QtWidgets/QWidget>
#include "ui_changeUserInfo.h"

class changeUserInfo : public QWidget
{
    Q_OBJECT

public:
    changeUserInfo(QWidget *parent = Q_NULLPTR);
	~changeUserInfo();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::changeUserInfoClass ui;
	//�����ƶ�����ֵ
	QPoint m_point;
	bool m_bMove = false;
};
