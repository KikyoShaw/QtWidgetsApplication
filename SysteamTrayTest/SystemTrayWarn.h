#pragma once

#include <QWidget>
#include <QMouseEvent>
#include "ui_SystemTrayWarn.h"

//�����ļ�����
const QString Common_Setting_FileName = "common.ini";

enum ExitStatus
{
	E_ToolTip = 0, //������ʾ
	E_Exit = 1, //ֱ���˳�
	E_Tray = 2, //��С������
};

class SystemTrayWarn : public QWidget
{
	Q_OBJECT

public:
	explicit SystemTrayWarn(QWidget *parent = Q_NULLPTR);
	~SystemTrayWarn();

private:
	//д�����ļ�
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
	//�����ƶ�����ֵ
	QPoint m_point;
	volatile bool m_bMove = false;
	//����
	ExitStatus m_status = ExitStatus::E_ToolTip;
};

