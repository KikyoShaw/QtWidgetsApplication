#include "animationTest.h"
#include <QPropertyAnimation>
#include "popAnimation.h"

animationTest::animationTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	m_pAnimation = new QPropertyAnimation(ui.label, "pos");
	if (m_pAnimation) {
		m_pAnimation->setDuration(2000);
		m_pAnimation->setStartValue(ui.label->pos());
		m_pAnimation->setEndValue(QPoint(-(ui.label->width()), ui.label->y()));
		//m_pAnimation->setEasingCurve(QEasingCurve::OutBounce);  // 缓和曲线风格
	}

	//m_pOpacityAnimation = new QPropertyAnimation(ui.label, "windowOpacity");
	//if (m_pOpacityAnimation) {
	//	m_pOpacityAnimation->setDuration(1000);
	//	m_pOpacityAnimation->setStartValue(1);
	//	m_pOpacityAnimation->setEndValue(0);
	//	connect(m_pOpacityAnimation, &QPropertyAnimation::finished, ui.label, &QLabel::hide);
	//}
	
	connect(ui.pushButton, &QPushButton::clicked, this, &animationTest::sltStartPopAnimation);
	connect(ui.pushButton2, &QPushButton::clicked, this, &animationTest::sltStartHideAnimation);
	connect(ui.pushButton3, &QPushButton::clicked, this, &animationTest::sltStartOpacityAnimation);
	connect(ui.pushButton4, &QPushButton::clicked, ui.label_rotation, &rotationLabel::startRotationAnimation);
}

animationTest::~animationTest()
{
}

void animationTest::sltStartHideAnimation()
{
	if (Q_NULLPTR == m_pAnimation)
		return;

	if(QAbstractAnimation::Running == m_pAnimation->state())
		return;

	m_pAnimation->start();
}

void animationTest::sltStartOpacityAnimation()
{
	//if (Q_NULLPTR == m_pOpacityAnimation)
	//	return;

	//if (QAbstractAnimation::Running == m_pOpacityAnimation->state())
	//	return;

	QWidget *widget = new QWidget();
	if (widget) {
		widget->setAttribute(Qt::WA_DeleteOnClose);
		widget->resize(100, 100);
		widget->show();

		QPropertyAnimation *pOpacityAnimation = new QPropertyAnimation(widget, "windowOpacity");
		if (pOpacityAnimation) {
			pOpacityAnimation->setDuration(3000);
			pOpacityAnimation->setStartValue(1);
			pOpacityAnimation->setEndValue(0);
			pOpacityAnimation->start();
			connect(pOpacityAnimation, &QPropertyAnimation::finished, widget, &QWidget::close);
		}
	}
}

void animationTest::sltStartPopAnimation()
{
	PopAnimation* popAnimation = new PopAnimation(this);
	if (popAnimation) {
		popAnimation->setAnimationSize(this->size());
		popAnimation->openAnimation();
		popAnimation->show();
	}
}
