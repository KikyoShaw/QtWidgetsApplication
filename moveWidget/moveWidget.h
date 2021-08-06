#pragma once

#ifndef TXTMOVETEST_H
#define TXTMOVETEST_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include "ui_moveWidget.h"

class MoveWidget : public QWidget
{
	Q_OBJECT

public:
	MoveWidget(QWidget *parent = Q_NULLPTR);
	~MoveWidget();

	void toStartMoveTimer(bool, int x = 0, int y = 0);

private:
	Ui::txtMoveTestWidget ui;
	QTimer m_MoveOut;
	QThread m_threadFloat;
	int m_posX = 0;
	int m_posY = 0;
	bool m_Direction = true;
};

#endif