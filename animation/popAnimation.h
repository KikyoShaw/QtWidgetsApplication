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

	//���õ���������ʾԪ�ظ���
	void setAnimationNum(int num);
	//���ö���������С
	void setAnimationSize(QSize size);
	//��������
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
	//��������
	QParallelAnimationGroup *m_animationGroup = Q_NULLPTR;
	//�ؼ�����
	QMap<QLabel*, QSize> m_labelMap;
	//���������
	QMap<QPropertyAnimation*, QSize> m_animationMap;
	//Ԫ�ظ���
	int m_animationNum;
	//�ƶ�����
	QParallelAnimationGroup *m_nMoveAnimationGroup = Q_NULLPTR;

};