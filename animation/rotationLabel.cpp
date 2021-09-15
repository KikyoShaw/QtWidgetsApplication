#include "rotationLabel.h"
#include <QPainter>
#include <QVariantAnimation>

rotationLabel::rotationLabel(QWidget *parent /* = Q_NULLPTR */)
	:QLabel(parent),
	m_matrixNum(0), m_imagePath(":/animationTest/qrc/th-c13.png")
{
	m_vAnimation = new QVariantAnimation(this);
	if (m_vAnimation) {
		m_vAnimation->setDuration(2000);
		m_vAnimation->setStartValue(0);
		m_vAnimation->setEndValue(360);
		m_vAnimation->setLoopCount(-1);
		connect(m_vAnimation, &QVariantAnimation::valueChanged, this, &rotationLabel::sltMatrixChanged);
	}
}

rotationLabel::~rotationLabel()
{
}

void rotationLabel::setImagePath(const QString & path)
{
	m_imagePath = path;
}

void rotationLabel::startRotationAnimation()
{
	if (Q_NULLPTR == m_vAnimation)
		return;

	if (QAbstractAnimation::Running == m_vAnimation->state()) {
		m_vAnimation->pause();
		return;
	}

	m_vAnimation->start();
}

void rotationLabel::sltMatrixChanged(QVariant value)
{
	// �����޸���ת�Ƕ�,���ػ�
	m_matrixNum = value.toInt();
	update();
}

void rotationLabel::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	// �����
	painter.setRenderHints(QPainter::Antialiasing, true);
	// ͼƬƽ������
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	//����ͼƬ��Դ
	QPixmap disc(m_imagePath);
	auto ateX = this->width() / 2;
	auto ateY = this->height() / 2;
	//�趨��ת���ĵ�
	painter.translate(ateX, ateY);
	//��ת�ĽǶ�
	painter.rotate(m_matrixNum);
	//�ָ����ĵ�
	painter.translate(-ateX, -ateY);
	//��ͼ����
	painter.drawPixmap(0, 0, this->width(), this->height(), disc);
}
