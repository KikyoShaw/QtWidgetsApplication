#pragma once

#include <QWidget>

class QPropertyAnimation;
class QParallelAnimationGroup;
class QLabel;

class PopAnimation : public QWidget
{
	Q_OBJECT
public:
	explicit PopAnimation(QWidget *parent = Q_NULLPTR);
	~PopAnimation();

	void setAnimationSize(QSize size);

	void openAnimation();

private:
	void initLabel();
	void initAnimation();
	void changeAnimationValue();

private:
	//动画管理
	QParallelAnimationGroup *m_animationGroup = Q_NULLPTR;
	//控件管理
	QVector<QLabel*> m_labelVec;
	//动画类管理
	QVector<QPropertyAnimation *> m_animationVec;
	//随机位置
	int m_posX;
	int m_posY;
};