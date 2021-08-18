#include "frameLessScale.h"
#include <QMouseEvent>

frameLessScale::frameLessScale(QWidget *parent)
    : FramelessWidget(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);

	//������
		//���ø������Ѵ�����ʾ����
	ui.splitter_main->setStretchFactor(0, 3);
	ui.splitter_main->setStretchFactor(1, 3);
	ui.splitter_main->setStretchFactor(2, 1);

	ui.splitter->setStretchFactor(0, 1);
	ui.splitter->setStretchFactor(1, 4);
	ui.splitter->setStretchFactor(2, 1);

	//�����ָ��ߵ�ʱ�򣬴����Ƿ�ʵʱ��ʾ
	ui.splitter->setOpaqueResize(false);
	// �����ӿؼ���ѹ������СΪ0
	ui.splitter->setChildrenCollapsible(false);
	//���ܷ��Ѵ����ڻ���һЩ�ؼ���Ϊ�˱�֤���ڲ��ؼ�����䶯��δ���ֵ�����£�������ͨ��QSplitter��splitterMoved�ź�ȥ������Щ�ؼ�resize
	connect(ui.splitter, &QSplitter::splitterMoved, this, [this]() {
		//xx(); ĳ�������ܽӿ�
	});
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
