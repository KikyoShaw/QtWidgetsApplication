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
	//��������
	QParallelAnimationGroup *m_animationGroup = Q_NULLPTR;
	//�ؼ�����
	QVector<QLabel*> m_labelVec;
	//���������
	QVector<QPropertyAnimation *> m_animationVec;
	//���λ��
	int m_posX;
	int m_posY;
};