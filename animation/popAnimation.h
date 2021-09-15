#pragma once

#include <QWidget>
#include <QMap>

class QPropertyAnimation;
class QParallelAnimationGroup;
class QLabel;

class PopAnimation : public QWidget
{
	Q_OBJECT
public:
	explicit PopAnimation(QWidget *parent = Q_NULLPTR);
	~PopAnimation();

	//设置单个动画显示元素个数
	void setAnimationNum(int num);
	//设置动画场景大小
	void setAnimationSize(QSize size);
	//启动动画
	void openAnimation();

private:
	void initLabel();
	void initAnimation();
	void changeAnimationValue();
	void initMoveAnimationModel();

private slots:
	void sltStartAnimationGroup();
	void sltStartAnimationOpacityEffect();

private:
	//动画管理
	QParallelAnimationGroup *m_animationGroup = Q_NULLPTR;
	//控件管理
	QMap<QLabel*, QSize> m_labelMap;
	//动画类管理
	QMap<QPropertyAnimation*, QSize> m_animationMap;
	//元素个数
	int m_animationNum;
	//移动动画
	QParallelAnimationGroup *m_nMoveAnimationGroup = Q_NULLPTR;

};