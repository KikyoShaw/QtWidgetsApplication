#include "frameLessScale.h"
#include <QMouseEvent>

frameLessScale::frameLessScale(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
}

frameLessScale::~frameLessScale()
{
}

void frameLessScale::mouseMoveEvent(QMouseEvent * event)
{
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);//移动窗口
	}
	QWidget::mouseMoveEvent(event);
}

void frameLessScale::mousePressEvent(QMouseEvent * event)
{
	//鼠标事件中包含“按住的是左键”
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		//获取移动的位移量
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void frameLessScale::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}
