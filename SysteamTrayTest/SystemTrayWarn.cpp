#include "SystemTrayWarn.h"
#include <QSettings>

SystemTrayWarn::SystemTrayWarn(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	setWindowModality(Qt::ApplicationModal);
	setWindowFlag(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	ui.radioButton_TrayMin->setChecked(true);
	connect(ui.pushButton_trayClose, &QPushButton::clicked, this, &QWidget::close);
	connect(ui.pushButton_traySure, &QPushButton::clicked, this, &SystemTrayWarn::sltSaveINI);
}

SystemTrayWarn::~SystemTrayWarn()
{
}

void SystemTrayWarn::writeINI()
{
	QSettings settings(Common_Setting_FileName, QSettings::IniFormat);
	//读取是否退出还是缩小到托盘
	m_status = ExitStatus::E_ToolTip;
	if (ui.checkBox_trayCheck->isChecked()) {
		if (ui.radioButton_TrayExit->isChecked()) {
			m_status = ExitStatus::E_Exit;
		}
		else if (ui.radioButton_TrayMin->isChecked()) {
			m_status = ExitStatus::E_Tray;
		}
	}
	settings.beginGroup("config");
	settings.setValue("status", m_status);
	settings.endGroup();
}

void SystemTrayWarn::sltSaveINI()
{
	//保存配置文件
	writeINI();
	//如果选择退出按钮直接退出
	if (ui.radioButton_TrayExit->isChecked()) {
		qApp->quit();
	}
	//选择缩小到托盘发送信号到上一层
	if (ui.radioButton_TrayMin->isChecked()) {
		emit sigMinToTray();
	}
	close();
}

void SystemTrayWarn::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void SystemTrayWarn::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void SystemTrayWarn::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}
