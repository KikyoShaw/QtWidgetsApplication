#include "frameLessScale.h"
#include <QMouseEvent>

frameLessScale::frameLessScale(QWidget *parent)
    : FramelessWidget(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);

	//分裂器
		//设置各个分裂窗口显示比例
	ui.splitter_main->setStretchFactor(0, 3);
	ui.splitter_main->setStretchFactor(1, 3);
	ui.splitter_main->setStretchFactor(2, 1);

	ui.splitter->setStretchFactor(0, 1);
	ui.splitter->setStretchFactor(1, 4);
	ui.splitter->setStretchFactor(2, 1);

	//拖拉分割线的时候，窗口是否实时显示
	ui.splitter->setOpaqueResize(false);
	// 不让子控件被压缩到大小为0
	ui.splitter->setChildrenCollapsible(false);
	//可能分裂窗口内还有一些控件，为了保证其内部控件跟随变动（未布局的情况下），可以通过QSplitter的splitterMoved信号去调用那些控件resize
	connect(ui.splitter, &QSplitter::splitterMoved, this, [this]() {
		//xx(); 某种需求功能接口
	});
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
