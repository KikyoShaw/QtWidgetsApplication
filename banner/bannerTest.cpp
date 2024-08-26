#include "bannerTest.h"
#include "QBannerWidget.h"
#include <QTimer>

bannerTest::bannerTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	/*setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);*/

	//轮播组件
	m_QBannerWidget = new QBannerWidget(ui.frame_banner);
	m_QBannerWidget->setImageSize(ui.frame_banner->size());
	m_QBannerWidget->addImage(":/banner/Resources/1.png");
	m_QBannerWidget->addImage(":/banner/Resources/2.png");
	m_QBannerWidget->addImage(":/banner/Resources/3.png");
	m_QBannerWidget->addImage(":/banner/Resources/4.png");
	m_QBannerWidget->addImage(":/banner/Resources/5.png");
	m_QBannerWidget->addImage(":/banner/Resources/6.png");
	m_QBannerWidget->addImage(":/banner/Resources/7.png");
	m_QBannerWidget->addImage(":/banner/Resources/8.png");


	//左按钮
	m_leftButton = new QPushButton(ui.frame_banner);
	m_leftButton->setFixedSize(50, 50);
	m_leftButton->setStyleSheet("border-image: url(:/banner/Resources/left.png);");
	int right_x = ui.frame_banner->width() - 20 - 50;
	int posY = (ui.frame->height() - 50) / 2;
	m_leftButton->move(20, posY);
	m_leftButton->setCursor(Qt::PointingHandCursor);
	connect(m_leftButton, SIGNAL(clicked()), this, SLOT(sltBannerNext()));
	m_leftButton->hide();

	//右按钮
	m_rightButton = new QPushButton(ui.frame_banner);
	m_rightButton->setFixedSize(50, 50);
	m_rightButton->setStyleSheet("border-image: url(:/banner/Resources/right.png);");
	m_rightButton->move(right_x, posY);
	m_rightButton->setCursor(Qt::PointingHandCursor);
	connect(m_rightButton, SIGNAL(clicked()), this, SLOT(sltBannerPrev()));
	m_rightButton->hide();

	//定时器
	m_bannerTimer = new QTimer(this);
	m_bannerTimer->setInterval(5000);
	connect(m_bannerTimer, SIGNAL(timeout()), m_QBannerWidget, SLOT(sltPrev()));
	m_bannerTimer->start();
}

bannerTest::~bannerTest()
{

}

void bannerTest::sltBannerNext()
{
	if (m_bannerTimer->isActive())
		m_bannerTimer->stop();

	m_QBannerWidget->sltNext();
	m_bannerTimer->start();
}

void bannerTest::sltBannerPrev()
{
	if (m_bannerTimer->isActive())
		m_bannerTimer->stop();

	m_QBannerWidget->sltPrev();
	m_bannerTimer->start();
}

void bannerTest::enterEvent(QEvent * event)
{
	Q_UNUSED(event);

	if(!m_leftButton->isVisible())
		m_leftButton->show();
	if(!m_rightButton->isVisible())
		m_rightButton->show();

	QWidget::enterEvent(event);
}

void bannerTest::leaveEvent(QEvent * event)
{
	Q_UNUSED(event);

	if (m_leftButton->isVisible())
		m_leftButton->hide();
	if (m_rightButton->isVisible())
		m_rightButton->hide();

	QWidget::leaveEvent(event);
}

void bannerTest::keyPressEvent(QKeyEvent * event)
{
	Q_UNUSED(event);

	if (event->key() == Qt::Key_Escape)
	{
		this->close();
	}

	QWidget::keyPressEvent(event);
}

void bannerTest::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void bannerTest::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void bannerTest::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}
