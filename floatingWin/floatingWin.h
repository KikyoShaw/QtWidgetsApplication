#pragma once

#include <QtWidgets/QWidget>
#include "ui_floatingWin.h"

class floatingWin : public QWidget
{
    Q_OBJECT

public:
    floatingWin(QWidget *parent = Q_NULLPTR);
	~floatingWin();

private:
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent(QEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::floatingWinClass ui;
	//窗口移动属性值
	QPoint m_point;
	bool m_bMove = false;
};
