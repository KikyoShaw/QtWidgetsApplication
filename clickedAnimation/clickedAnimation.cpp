#include "clickedAnimation.h"
#include <QMouseEvent>
#include "WaterClicked.h"
#include <QPropertyAnimation>
#include <QLabel>
#include <QDebug>
#include "imageLabel.h"

#include <QGraphicsScene>
#include <QGraphicsView>

clickedAnimation::clickedAnimation(QWidget *parent)
    : QWidget(parent), m_clickedPoint(QPoint(145, 165))
{
    ui.setupUi(this);

	m_bgLabel = new QLabel(this);
	//m_bgLabel->setStyleSheet("border-image: url(:/clickedAnimation/qrc/icon_sz2.png);");
	m_bgLabel->resize(48, 48);

	m_imageLabel = new ImageLabel(this);
	m_imageLabel->resize(60, 60);
	m_imageLabel->setImagePath(":/clickedAnimation/qrc/icon_sz2.png");

	//QGraphicsScene* pScene = new QGraphicsScene(this);
	//pScene->addWidget(ui.label);
	//QGraphicsView* pView = new QGraphicsView(pScene, ui.label);
	//pView->setFixedSize(44, 48);
	//pView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//pView->rotate(100);

	/* ����һ���������� */
	m_animation = new QPropertyAnimation(this);
	if (m_animation) {
		/* ���ö���Ŀ�� */
		m_animation->setTargetObject(m_imageLabel);
		/* ���ô��ڵ�λ����Ϊ�����ο� */
		m_animation->setPropertyName("pos");
		/* ���ö�������ʱ�� */
		m_animation->setDuration(800);
		///* ���ö�����ʼλ�� */
		m_animation->setStartValue(QPoint(150, 170));
		///* ���ö�������λ�� */
		m_animation->setEndValue(QPoint(140, 160));
		//m_animation->setKeyValueAt(0, QPoint(150, 170));
		//m_animation->setKeyValueAt(0.5, QPoint(145, 165));
		//m_animation->setKeyValueAt(1, QPoint(140, 160));
		/* ����ѭ��������-1Ϊ���޴� */
		m_animation->setLoopCount(-1);
		/* ��ʼ���� */
		m_animation->start();
		connect(m_animation, &QVariantAnimation::valueChanged, this, &clickedAnimation::sltValueChanged);
	}
}

clickedAnimation::~clickedAnimation()
{
}

void clickedAnimation::setWaterClicked(QPoint point)
{
	WaterDrop* waterDrop = new WaterDrop(this);
	if (waterDrop) {
		waterDrop->move(point);
		/*waterDrop->move(this->mapToGlobal(point));*/
		waterDrop->show();
	}
}

void clickedAnimation::setMatrix(int radius)
{
	//m_bgLabel->setPixmap(QPixmap());
	//matrix.rotate(radius);
	//QPixmap logo(":/clickedAnimation/qrc/icon_sz2.png");
	//if (!logo.isNull()) {
	//	m_bgLabel->setPixmap(logo.transformed(matrix, Qt::SmoothTransformation));
	//}
	if (m_imageLabel) {
		m_imageLabel->setMatrix(radius);
	}

}

void clickedAnimation::sltValueChanged(const QVariant &value)
{
	auto point = value.toPoint();
	//qInfo() << value;
	if (point == QPoint(143, 163)) {
		setWaterClicked(QPoint(150, 172));
	}
	if (point == QPoint(144, 164)) {
		setWaterClicked(QPoint(150, 172));
	}

	if (point.x() > m_clickedPoint.x()){
		setMatrix(10);
	}
	else {
		setMatrix(350);
	}

}

void clickedAnimation::mousePressEvent(QMouseEvent * event)
{
	QPoint cursorPos = event->pos();
	WaterDrop* waterDrop = new WaterDrop();
	waterDrop->move(this->mapToGlobal(cursorPos));
	waterDrop->show();
}
