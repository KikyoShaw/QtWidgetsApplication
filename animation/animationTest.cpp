#include "animationTest.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#define POSWIDTH 40
#define POSHEIGHT 40
#define MAXWIDTH 100
#define MAXHEIGHT 100


animationTest::animationTest(QWidget *parent)
    : QWidget(parent), m_posX(0), m_posY(0)
{
    ui.setupUi(this);

	m_posX = (this->width() - POSWIDTH) / 2;
	m_posY = (this->height() - POSHEIGHT) / 2;

	m_animationGroup = new QParallelAnimationGroup(this);

	initLabel();
	initAnimation();

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

void animationTest::initLabel()
{
	for (int i = 0; i < 10; i++) {
		QLabel *label = new QLabel(ui.frame);
		if (label) {
			label->resize(POSWIDTH, POSHEIGHT);
			label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
			auto style = QString("border-image: url(:/animationTest/qrc/th-c%1.png);").arg(i + 1);
			label->setStyleSheet(style);
			label->move(m_posX, m_posY);
			m_labelVec.append(label);
		}
	}
}

void animationTest::initAnimation()
{
	for (auto & each : m_labelVec) {
		QPropertyAnimation * pAnimation = new QPropertyAnimation(each, "geometry");
		if (pAnimation) {
			pAnimation->setDuration(1000);
			//InOutElastic:抖动后散开、 OutBounce:缓和曲线风格、InBounce:散开合并散开
			pAnimation->setEasingCurve(QEasingCurve::OutBounce);
			m_animationGroup->addAnimation(pAnimation);
			m_animationVec.append(pAnimation);
		}
	}
}

void animationTest::changeAnimationValue()
{
	for (auto &each : m_animationVec) {
		//防止控件越界
		auto descX = this->width() - MAXWIDTH;
		auto descY = this->height() - MAXHEIGHT;
		//选取随机位置
		auto posX = qrand() % descX;
		auto posY = qrand() % descY;
		each->setStartValue(QRect(m_posX, m_posY, POSWIDTH, POSHEIGHT));
		each->setEndValue(QRect(posX, posY, MAXWIDTH, MAXHEIGHT));
	}
}

void animationTest::sltStartAnimation()
{
	if (Q_NULLPTR == m_animationGroup)
		return;
	//先判断动画是否结束
	/*if (QAbstractAnimation::Running == m_pAnimation->state())
		return;*/
	if (QAbstractAnimation::Running == m_animationGroup->state())
		return;

	changeAnimationValue();
	m_animationGroup->start();

	////防止控件越界
	//auto descX = this->width() - ui.label->width();
	//auto descY = this->height() - ui.label->height();
	////选取随机位置
	//auto posX = qrand() % descX;
	//auto posY = qrand() % descY;
	////动画工作
	//m_pAnimation->setStartValue(QRect(m_posX, m_posY, 60, 60));
	//m_pAnimation->setEndValue(QRect(posX, posY, 60, 60));
	//m_pAnimation->start();
	////记住上一次值
	//m_posX = posX;
	//m_posY = posY;
}
