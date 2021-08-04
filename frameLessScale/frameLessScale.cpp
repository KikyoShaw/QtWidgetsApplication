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
		move(event->globalPos() - m_point);//�ƶ�����
	}
	QWidget::mouseMoveEvent(event);
}

void frameLessScale::mousePressEvent(QMouseEvent * event)
{
	//����¼��а�������ס���������
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		//��ȡ�ƶ���λ����
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void frameLessScale::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}
