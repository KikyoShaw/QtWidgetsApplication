#pragma once

#include <QtWidgets/QWidget>
#include "ui_heartProgress.h"

class QPainter;

class heartProgress : public QWidget
{
    Q_OBJECT

public:
    heartProgress(QWidget *parent = Q_NULLPTR);
	~heartProgress();

private:
	//绘制红心样式
	void paintEvent(QPaintEvent *event);
	//绘制中间背景
	void paintBg(QPainter* painter);
	//绘制进度文字
	void paintText(QPainter* painter);

private:
    Ui::heartProgressClass ui;
};
