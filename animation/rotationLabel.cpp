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
	// 不断修改旋转角度,并重绘
	m_matrixNum = value.toInt();
	update();
}

void rotationLabel::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 图片平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	//加载图片资源
	QPixmap disc(m_imagePath);
	auto ateX = this->width() / 2;
	auto ateY = this->height() / 2;
	//设定旋转中心点
	painter.translate(ateX, ateY);
	//旋转的角度
	painter.rotate(m_matrixNum);
	//恢复中心点
	painter.translate(-ateX, -ateY);
	//画图操作
	painter.drawPixmap(0, 0, this->width(), this->height(), disc);
}
