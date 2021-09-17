#include "animationTest.h"
#include <QPropertyAnimation>
#include "popAnimation.h"
#include <QGraphicsDropShadowEffect>
#include <QtMath>

animationTest::animationTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	//移动动画
	m_pAnimation = new QPropertyAnimation(ui.label, "pos");
	if (m_pAnimation) {
		m_pAnimation->setDuration(2000);
		//m_pAnimation->setLoopCount(-1);
		connect(m_pAnimation, &QVariantAnimation::finished, this, &animationTest::sltMoveChanged);
		//m_pAnimation->setEasingCurve(QEasingCurve::OutBounce);  // 缓和曲线风格
	}

	//初始化阴影透明控件动画
	initShadowEffect();

	//曲线运动
	initCurvilinear();
	
	connect(ui.pushButton, &QPushButton::clicked, this, &animationTest::sltStartPopAnimation);
	connect(ui.pushButton2, &QPushButton::clicked, this, &animationTest::sltStartHideAnimation);
	connect(ui.pushButton3, &QPushButton::clicked, this, &animationTest::sltStartOpacityAnimation); 
	connect(ui.pushButton4, &QPushButton::clicked, ui.label_rotation, &rotationLabel::startRotationAnimation);
	connect(ui.pushButton5, &QPushButton::clicked, this, &animationTest::sltStartShadowEffectAnimation);
	connect(ui.pushButton6, &QPushButton::clicked, this, &animationTest::sltStartCurAnimation);
	connect(ui.pushButton7, &QPushButton::clicked, this, &animationTest::sltStartOpacityEffectAnimation);
	connect(ui.pushButton8, &QPushButton::clicked, ui.widget_comBo, &ComBo::startComBoAnimation);
}

animationTest::~animationTest()
{
}

void animationTest::initShadowEffect()
{
	//动画
	m_vShadowEffectAnimation = new QVariantAnimation(this);
	if (m_vShadowEffectAnimation) {
		m_vShadowEffectAnimation->setDuration(2000);
		m_vShadowEffectAnimation->setKeyValueAt(0, 0);
		m_vShadowEffectAnimation->setKeyValueAt(0.5, 100);
		m_vShadowEffectAnimation->setKeyValueAt(1, 0);
		m_vShadowEffectAnimation->setLoopCount(-1);
		connect(m_vShadowEffectAnimation, &QVariantAnimation::valueChanged, this, &animationTest::sltShadowEffectRaduisChanged);
	}
	//设置边框阴影
	m_shadowEffect = new QGraphicsDropShadowEffect(this);
	if (m_shadowEffect) {
		m_shadowEffect->setColor(QColor(0, 0, 139));
		m_shadowEffect->setOffset(0, 0);
		m_shadowEffect->setBlurRadius(0);
		ui.label_shadowEffect->setGraphicsEffect(m_shadowEffect);
	}

	//设置边框阴影
	m_opacityEffect = new QGraphicsOpacityEffect(this);
	if (m_opacityEffect) {
		ui.label_shadowEffect_2->setGraphicsEffect(m_opacityEffect);
		m_opacityEffect->setOpacity(1);
	}
	//动画
	m_vOpacityEffectAnimation = new QVariantAnimation(this);
	if (m_vOpacityEffectAnimation) {
		m_vOpacityEffectAnimation->setDuration(400);
		m_vOpacityEffectAnimation->setKeyValueAt(0, 1.0);
		m_vOpacityEffectAnimation->setKeyValueAt(0.5, 0.0);
		m_vOpacityEffectAnimation->setKeyValueAt(1, 1.0);
		m_vOpacityEffectAnimation->setLoopCount(-1);
		connect(m_vOpacityEffectAnimation, &QVariantAnimation::valueChanged, this, [=](QVariant value) {
			if (m_opacityEffect) {
				m_opacityEffect->setOpacity(value.toDouble());
			}
		});
	}
}

void animationTest::initCurvilinear()
{
	m_pCurAnimation = new QPropertyAnimation(ui.label_curvilinear, "geometry");
	if (m_pCurAnimation) {
		m_pCurAnimation->setDuration(1500);
		double a = qDegreesToRadians(30.0);
		//int A = 920 - 310;
		//int B = 500 - 270;
		//int R = A * B / sqrt(pow(A*sin(a), 2) + pow(B*cos(a), 2)); //计算对应角度的半径
		//int x = 0 + R * cos(a);
		//int y = 0 + R * sin(a);
		//m_pCurAnimation->setKeyValueAt(0, QRect(920, 500, 100, 100));
		//m_pCurAnimation->setKeyValueAt(0.5, QRect(x + 310, y + 270, 100, 100));
		//m_pCurAnimation->setKeyValueAt(1, QRect(310, 270, 100, 100));

		m_pCurAnimation->setStartValue(QRect(920, 500, 100, 100));
		m_pCurAnimation->setEndValue(QRect(410, 270, 100, 100));

		m_pCurAnimation->setKeyValueAt(0, QRect(920, 500, 100, 100));
		m_pCurAnimation->setKeyValueAt(0.5, QRect(500, 300, 100, 100));
		m_pCurAnimation->setKeyValueAt(1, QRect(310, 270, 100, 100));

		//InOutElastic:抖动后散开、 OutBounce:缓和曲线风格、InBounce:散开合并散开
		m_pCurAnimation->setEasingCurve(QEasingCurve::NCurveTypes);
	}
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

		QLabel *label = new QLabel(widget);
		if (label) {
			label->resize(80, 80);
			label->setStyleSheet("border-image: url(:/animationTest/qrc/th-c1.png);");
			label->show();
			/*auto moveX = widget->width() - label->width();
			auto moveY = widget->height() - label->height();
			label->move(moveX, moveY);*/
		}

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

void animationTest::sltStartOpacityEffectAnimation()
{
	if (Q_NULLPTR == m_vOpacityEffectAnimation)
		return;

	if (QAbstractAnimation::Running == m_vOpacityEffectAnimation->state()) {
		m_vOpacityEffectAnimation->pause();
		return;
	}

	m_vOpacityEffectAnimation->start();
}

void animationTest::sltStartCurAnimation()
{
	if (Q_NULLPTR == m_pCurAnimation)
		return;

	if (QAbstractAnimation::Running == m_pCurAnimation->state()) {
		return;
	}

	m_pCurAnimation->start();
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
		popAnimation->setAnimationNum(12);
		popAnimation->openAnimation();
		popAnimation->show();
	}
}
