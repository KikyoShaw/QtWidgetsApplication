#pragma once

#include <QtWidgets/QWidget>
#include "ui_frameLessScale.h"

class frameLessScale : public QWidget
{
    Q_OBJECT

public:
    frameLessScale(QWidget *parent = Q_NULLPTR);
	~frameLessScale();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::frameLessScaleClass ui;
	//¥∞ø⁄“∆∂Ø Ù–‘
	QPoint m_point;
	bool m_bMove = false;
};
