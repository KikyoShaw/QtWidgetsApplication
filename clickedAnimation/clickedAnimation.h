#pragma once

#include <QtWidgets/QWidget>
#include "ui_clickedAnimation.h"

class QPropertyAnimation;
class ImageLabel;
class QLabel;

class clickedAnimation : public QWidget
{
    Q_OBJECT

public:
    clickedAnimation(QWidget *parent = Q_NULLPTR);
	~clickedAnimation();

private:
	void setWaterClicked(QPoint point);

	void setMatrix(int radius);

private slots:
	void sltValueChanged(const QVariant &value);

private:
	virtual void mousePressEvent(QMouseEvent *event);

private:
    Ui::clickedAnimationClass ui;

	//�ƶ��ؼ�
	QLabel *m_bgLabel = nullptr;

	//������
	QPropertyAnimation *m_animation = nullptr;

	//���λ��
	QPoint m_clickedPoint;

	//��ת
	QMatrix matrix;

	//�ƶ��ؼ�
	ImageLabel *m_imageLabel = nullptr;

};
