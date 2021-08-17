#include "SysteamTrayTest.h"
#include <QMouseEvent>
#include "SysteamTrayManager.h"

SysteamTrayTest::SysteamTrayTest(QWidget *parent)
    : QWidget(parent),m_point(QPoint()), m_isMove(false)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	systeamTrayManager.initSysteamTray();
	systeamTrayManager.setUserInfoOnTray();

	connect(ui.pushButton, &QPushButton::clicked, this, &QWidget::hide);
	//系统托盘打开总控台
	connect(&systeamTrayManager, &SysteamTrayManager::sigTrayToOpen, this, &QWidget::show);
}

SysteamTrayTest::~SysteamTrayTest()
{
}

void SysteamTrayTest::mouseMoveEvent(QMouseEvent * event)
{
	if ((event->buttons() & Qt::LeftButton) && m_isMove) {
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void SysteamTrayTest::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		m_isMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void SysteamTrayTest::mouseReleaseEvent(QMouseEvent * event)
{
	m_isMove = false;
	QWidget::mouseReleaseEvent(event);
}
