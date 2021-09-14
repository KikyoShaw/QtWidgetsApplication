#pragma once

#include <QtWidgets/QWidget>
#include "ui_animationTest.h"

class QPropertyAnimation;
class QParallelAnimationGroup;

class animationTest : public QWidget
{
    Q_OBJECT

public:
    explicit animationTest(QWidget *parent = Q_NULLPTR);
	~animationTest();

private:
	void initLabel();
	void initAnimation();
	void changeAnimationValue();

private slots:
	void sltStartAnimation();

private:
    Ui::animationTestClass ui;
	//������Ч
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
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
