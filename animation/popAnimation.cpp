#include "popAnimation.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QLabel>

#define POSWIDTH 60
#define POSHEIGHT 60
#define MAXWIDTH 100
#define MAXHEIGHT 100

PopAnimation::PopAnimation(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent), m_posX(0), m_posY(0)
{
	this->setWindowFlags(Qt::FramelessWindowHint /*| Qt::Tool*/);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAttribute(Qt::WA_TransparentForMouseEvents, true);

	// 控件显示完关闭后自动删除
	setAttribute(Qt::WA_DeleteOnClose);

	m_animationGroup = new QParallelAnimationGroup(this);
	if (m_animationGroup) {
		connect(m_animationGroup, &QParallelAnimationGroup::finished, this, &PopAnimation::close);
	}

	initLabel();
	initAnimation();
}

PopAnimation::~PopAnimation()
{
}

void PopAnimation::setAnimationSize(QSize size)
{
	this->resize(size);
	m_posX = (this->width() - POSWIDTH) / 2;
	m_posY = (this->height() - POSHEIGHT) / 2;
}

void PopAnimation::openAnimation()
{
	if (Q_NULLPTR == m_animationGroup)
		return;

	changeAnimationValue();
	m_animationGroup->start();
}

void PopAnimation::initLabel()
{
	for (int i = 0; i < 10; i++) {
		QLabel *label = new QLabel(this);
		if (label) {
			label->resize(POSWIDTH, POSHEIGHT);
			auto style = QString("border-image: url(:/animationTest/qrc/th-c%1.png);").arg(i + 1);
			label->setStyleSheet(style);
			label->move(m_posX, m_posY);
			m_labelVec.append(label);
		}
	}
}

void PopAnimation::initAnimation()
{
	for (auto & each : m_labelVec) {
		QPropertyAnimation * pAnimation = new QPropertyAnimation(each, "geometry");
		if (pAnimation) {
			pAnimation->setDuration(1500);
			//InOutElastic:抖动后散开、 OutBounce:缓和曲线风格、InBounce:散开合并散开
			pAnimation->setEasingCurve(QEasingCurve::OutBounce);
			m_animationGroup->addAnimation(pAnimation);
			m_animationVec.append(pAnimation);
		}
	}
}

void PopAnimation::changeAnimationValue()
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
