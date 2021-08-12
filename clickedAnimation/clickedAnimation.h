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

	//移动控件
	QLabel *m_bgLabel = nullptr;

	//动画类
	QPropertyAnimation *m_animation = nullptr;

	//点击位置
	QPoint m_clickedPoint;

	//旋转
	QMatrix matrix;

	//移动控件
	ImageLabel *m_imageLabel = nullptr;

};
