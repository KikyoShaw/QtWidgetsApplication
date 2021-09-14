#include "animationTest.h"
#include <QPropertyAnimation>
#include "popAnimation.h"

animationTest::animationTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	m_pAnimation = new QPropertyAnimation(ui.label, "geometry");
	if (m_pAnimation) {
		m_pAnimation->setDuration(1000);
		//m_animationGroup->addAnimation(m_pAnimation);
		//m_pAnimation->setEasingCurve(QEasingCurve::OutBounce);  // 缓和曲线风格
	}

	connect(ui.pushButton, &QPushButton::clicked, this, &animationTest::sltStartAnimation);
}

animationTest::~animationTest()
{
}

void animationTest::sltStartAnimation()
{
	PopAnimation* popAnimation = new PopAnimation(this);
	if (popAnimation) {
		popAnimation->setAnimationSize(this->size());
		popAnimation->openAnimation();
		popAnimation->show();
	}
}
