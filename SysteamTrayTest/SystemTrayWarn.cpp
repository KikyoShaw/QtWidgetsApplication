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
	//��ȡ�Ƿ��˳�������С������
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
	//���������ļ�
	writeINI();
	//���ѡ���˳���ťֱ���˳�
	if (ui.radioButton_TrayExit->isChecked()) {
		qApp->quit();
	}
	//ѡ����С�����̷����źŵ���һ��
	if (ui.radioButton_TrayMin->isChecked()) {
		emit sigMinToTray();
	}
	close();
}

void SystemTrayWarn::mouseMoveEvent(QMouseEvent * event)
{
	//�ж�����Ƿ񱻰��£�ֻ����������ˣ��䷵��ֵ����1(true)
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
