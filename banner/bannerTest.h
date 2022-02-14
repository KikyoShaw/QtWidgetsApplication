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

	//窗口移动属性值
	QPoint m_point;
	bool m_bMove = false;

	//轮播组件
	QBannerWidget *m_QBannerWidget = nullptr;

	//左右按钮
	QPushButton *m_leftButton = nullptr;
	QPushButton *m_rightButton = nullptr;

	//定时器刷新
	QTimer *m_bannerTimer = nullptr;
};
