#include "popAnimation.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QPainter>

#define ONESIZE QSize(60, 60)
#define TWOSIZE QSize(80, 80)

PopAnimation::PopAnimation(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent)
{
	this->setWindowFlags(Qt::FramelessWindowHint /*| Qt::Tool*/);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAttribute(Qt::WA_TransparentForMouseEvents, true);

	// �ؼ���ʾ��رպ��Զ�ɾ��
	setAttribute(Qt::WA_DeleteOnClose);

	//��ʼ��������
	m_nMoveAnimationGroup = new QParallelAnimationGroup(this);
	if (m_nMoveAnimationGroup) {
		connect(m_nMoveAnimationGroup, &QParallelAnimationGroup::finished, this, &PopAnimation::sltStartAnimationGroup);
	}

	m_animationGroup = new QParallelAnimationGroup(this);
	if (m_animationGroup) {
		connect(m_animationGroup, &QParallelAnimationGroup::finished, this, &PopAnimation::close);
	}

	m_labelMap.clear();
	m_animationMap.clear();
}

PopAnimation::~PopAnimation()
{
}

void PopAnimation::setAnimationNum(int num)
{
	m_animationNum = num;
	//��ʼ���ؼ�
	initLabel();
	initAnimation();
}

void PopAnimation::setAnimationSize(QSize size)
{
	this->resize(size);
	//��ʼ���ƶ��������
	initMoveAnimationModel();
}

void PopAnimation::openAnimation()
{
	if (Q_NULLPTR == m_nMoveAnimationGroup)
		return;

	m_nMoveAnimationGroup->start();
}

void PopAnimation::initLabel()
{
	//���С�Ķ԰��
	auto index = m_animationNum / 2;
	for (int i = 0; i < m_animationNum; i++) {
		QLabel *label = new QLabel(this);
		if (label) {
			if (i < index) {
				label->resize(ONESIZE);
				auto moveX = (this->width() - ONESIZE.width()) / 2;
				auto moveY = (this->height() - ONESIZE.height()) / 2;
				label->move(moveX, moveY);
			}
			else {
				label->resize(TWOSIZE);
				auto moveX = (this->width() - TWOSIZE.width()) / 2;
				auto moveY = (this->height() - TWOSIZE.height()) / 2;
				label->move(moveX, moveY);
			}
			auto style = QString("border-image: url(:/animationTest/qrc/th-c%1.png);").arg(i + 1);
			label->setStyleSheet(style);
			label->hide();
			m_labelMap.insert(label, label->size());
		}
	}
}

void PopAnimation::initAnimation()
{
	auto labels = m_labelMap.keys();
	for (auto & each : labels) {
		QPropertyAnimation * pAnimation = new QPropertyAnimation(each, "geometry");
		if (pAnimation) {
			pAnimation->setDuration(1500);
			//InOutElastic:������ɢ���� OutBounce:�������߷��InBounce:ɢ���ϲ�ɢ��,TCBSpline
			pAnimation->setEasingCurve(QEasingCurve::InQuad);
			m_animationGroup->addAnimation(pAnimation);
			auto size = m_labelMap.value(each);
			m_animationMap.insert(pAnimation, size);
		}
	}
}

void PopAnimation::changeAnimationValue()
{
	auto animations = m_animationMap.keys();
	for (auto &each : animations) {
		auto size = m_animationMap.value(each);
		//��ֹ�ؼ�Խ��
		auto descX = this->width() - size.width();
		auto descY = this->height() - size.height();
		//ѡȡ���λ��
		auto posX = qrand() % descX;
		auto posY = qrand() % descY;
		each->setStartValue(QRect(descX / 2, descY / 2, size.width(), size.width()));
		each->setEndValue(QRect(posX, posY, size.width(), size.width()));
	}
}

void PopAnimation::initMoveAnimationModel()
{
	//�ȴ�������ʼ�����½ǿؼ�
	//������this����Ϊ����������ʧ
	QLabel *label = new QLabel(this);
	if (label) {
		//label->setAttribute(Qt::WA_DeleteOnClose);
		label->resize(TWOSIZE);
		label->setStyleSheet("border-image: url(:/animationTest/qrc/th-c1.png);");
		auto moveX = this->width() - label->width();
		auto moveY = this->height() - label->height();
		label->move(moveX, moveY);
		/*label->show();*/

		//�����������ƶ�����
		QPropertyAnimation *moveAnimation = new QPropertyAnimation(label, "pos");
		if (moveAnimation) {
			moveAnimation->setDuration(1000);
			moveAnimation->setStartValue(label->pos());
			moveAnimation->setEndValue(QPoint(moveX / 2, moveY / 2));
			m_nMoveAnimationGroup->addAnimation(moveAnimation);
		}

		//����͸����Ӱʵ��
		QGraphicsOpacityEffect *pGraphicsOpacityEffect = new QGraphicsOpacityEffect();
		if (pGraphicsOpacityEffect) {
			label->setGraphicsEffect(pGraphicsOpacityEffect);
			pGraphicsOpacityEffect->setOpacity(1);
		}
		//��󴴽���������
		QVariantAnimation *pOpacityAnimation = new QVariantAnimation(this);
		if (pOpacityAnimation) {
			pOpacityAnimation->setDuration(1000);
			pOpacityAnimation->setStartValue(1.0);
			pOpacityAnimation->setEndValue(0.0);
			connect(pOpacityAnimation, &QVariantAnimation::valueChanged, this, [=](QVariant value) {
				if (pGraphicsOpacityEffect) {
					pGraphicsOpacityEffect->setOpacity(value.toDouble());
				}
			});
			m_nMoveAnimationGroup->addAnimation(pOpacityAnimation);
		}
		connect(m_nMoveAnimationGroup, &QParallelAnimationGroup::finished, label, &QLabel::close);
	}	
}

void PopAnimation::sltStartAnimationOpacityEffect()
{
	//����͸����Ӱʵ��
	QGraphicsOpacityEffect *animationOpacityEffect = new QGraphicsOpacityEffect(this);
	if (animationOpacityEffect) {
		this->setGraphicsEffect(animationOpacityEffect);
		animationOpacityEffect->setOpacity(1);
	}
	//������������
	QVariantAnimation *opacityEffectAnimation = new QVariantAnimation(this);
	if (opacityEffectAnimation) {
		opacityEffectAnimation->setDuration(1500);
		opacityEffectAnimation->setKeyValueAt(0, 1.0);
		opacityEffectAnimation->setKeyValueAt(0.9, 1.0);
		opacityEffectAnimation->setKeyValueAt(1, 0.0);
		m_animationGroup->addAnimation(opacityEffectAnimation);
		connect(opacityEffectAnimation, &QVariantAnimation::valueChanged, this, [=](QVariant value) {
			if (animationOpacityEffect) {
				animationOpacityEffect->setOpacity(value.toDouble());
			}
		});
	}
}

void PopAnimation::sltStartAnimationGroup()
{
	for (auto &each : m_labelMap.keys()) {
		if (each) {
			each->show();
		}
	}
	changeAnimationValue();
	sltStartAnimationOpacityEffect();
	m_animationGroup->start();
}
