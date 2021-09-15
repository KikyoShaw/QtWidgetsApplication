#include "animationTest.h"
#include <QPropertyAnimation>
#include "popAnimation.h"
#include <QGraphicsDropShadowEffect>

animationTest::animationTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	m_pAnimation = new QPropertyAnimation(ui.label, "pos");
	if (m_pAnimation) {
		m_pAnimation->setDuration(2000);
		//m_pAnimation->setLoopCount(-1);
		connect(m_pAnimation, &QVariantAnimation::finished, this, &animationTest::sltMoveChanged);
		//m_pAnimation->setEasingCurve(QEasingCurve::OutBounce);  // 缓和曲线风格
	}

	m_vShadowEffectAnimation = new QVariantAnimation(this);
	if (m_vShadowEffectAnimation) {
		m_vShadowEffectAnimation->setDuration(2000);
		m_vShadowEffectAnimation->setKeyValueAt(0, 0);
		m_vShadowEffectAnimation->setKeyValueAt(0.5, 100);
		m_vShadowEffectAnimation->setKeyValueAt(1, 0);
		m_vShadowEffectAnimation->setLoopCount(-1);
		connect(m_vShadowEffectAnimation, &QVariantAnimation::valueChanged, this, &animationTest::sltShadowEffectRaduisChanged);
	}

	m_shadowEffect = new QGraphicsDropShadowEffect(this);
	//设置边框阴影
	if (m_shadowEffect) {
		m_shadowEffect->setColor(QColor(0, 0, 139));
		m_shadowEffect->setOffset(0, 0);
		m_shadowEffect->setBlurRadius(0);
		ui.label_shadowEffect->setGraphicsEffect(m_shadowEffect);
	}
	
	connect(ui.pushButton, &QPushButton::clicked, this, &animationTest::sltStartPopAnimation);
	connect(ui.pushButton2, &QPushButton::clicked, this, &animationTest::sltStartHideAnimation);
	connect(ui.pushButton3, &QPushButton::clicked, this, &animationTest::sltStartOpacityAnimation);
	connect(ui.pushButton4, &QPushButton::clicked, ui.label_rotation, &rotationLabel::startRotationAnimation);
	connect(ui.pushButton5, &QPushButton::clicked, this, &animationTest::sltStartShadowEffectAnimation);
}

animationTest::~animationTest()
{
}

void animationTest::sltStartHideAnimation()
{
	if (Q_NULLPTR == m_pAnimation)
		return;

	if (QAbstractAnimation::Running == m_pAnimation->state()) {
		m_pAnimation->pause();
		return;
	}
	else if (QAbstractAnimation::Paused == m_pAnimation->state()) {
		m_pAnimation->start();
		return;
	}

	m_pAnimation->setStartValue(QPoint(768, 640));
	m_pAnimation->setEndValue(QPoint(247, ui.label->y()));

	m_pAnimation->start();
}

void animationTest::sltStartOpacityAnimation()
{
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

void animationTest::sltStartShadowEffectAnimation()
{
	if (Q_NULLPTR == m_vShadowEffectAnimation)
		return;

	if (QAbstractAnimation::Running == m_vShadowEffectAnimation->state()) {
		m_vShadowEffectAnimation->pause();
		return;
	}

	m_vShadowEffectAnimation->start();
}

void animationTest::sltShadowEffectRaduisChanged(QVariant value)
{
	auto radius = value.toInt();
	if (m_shadowEffect) {
		m_shadowEffect->setBlurRadius(radius);
	}
}

void animationTest::sltMoveChanged()
{
	//判断方位
	auto posX = ui.label->x();
	auto posY = ui.label->y();
	m_pAnimation->setStartValue(ui.label->pos());
	//向上移动
	if (247 == posX && 640 == posY) {
		m_pAnimation->setEndValue(QPoint(posX, 120));
	}
	//向右移动
	if (247 == posX && 120 == posY) {
		m_pAnimation->setEndValue(QPoint(768, posY));
	}
	//向下移动
	if (768 == posX && 120 == posY) {
		m_pAnimation->setEndValue(QPoint(posX, 640));
	}
	//向左移动
	if (768 == posX && 640 == posY) {
		m_pAnimation->setEndValue(QPoint(247, posY));
	}

	if (QAbstractAnimation::Stopped == m_pAnimation->state())
		m_pAnimation->start();
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
