#pragma once

#include <QWidget>
#include <QMouseEvent>
#include "ui_SystemTrayWarn.h"

//配置文件名称
const QString Common_Setting_FileName = "common.ini";

enum ExitStatus
{
	E_ToolTip = 0, //窗口提示
	E_Exit = 1, //直接退出
	E_Tray = 2, //缩小到托盘
};

class SystemTrayWarn : public QWidget
{
	Q_OBJECT

public:
	explicit SystemTrayWarn(QWidget *parent = Q_NULLPTR);
	~SystemTrayWarn();

private:
	//写配置文件
	void writeINI();

private slots:
	void sltSaveINI();

signals:
	void sigMinToTray();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
	Ui::systemTrayWarn ui;
	//窗口移动属性值
	QPoint m_point;
	volatile bool m_bMove = false;
	//属性
	ExitStatus m_status = ExitStatus::E_ToolTip;
};

