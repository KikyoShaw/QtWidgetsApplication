#pragma once

#include <QtWidgets/QWidget>
#include "ui_bannerTest.h"
#include <QPushButton>

class QBannerWidget;
class QTimer;

class bannerTest : public QWidget
{
    Q_OBJECT

public:
	bannerTest(QWidget *parent = Q_NULLPTR);
	~bannerTest();

private slots:
	void sltBannerNext();
	void sltBannerPrev();

private:
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::bannerClass ui;

	//�����ƶ�����ֵ
	QPoint m_point;
	bool m_bMove = false;

	//�ֲ����
	QBannerWidget *m_QBannerWidget = nullptr;

	//���Ұ�ť
	QPushButton *m_leftButton = nullptr;
	QPushButton *m_rightButton = nullptr;

	//��ʱ��ˢ��
	QTimer *m_bannerTimer = nullptr;
};
