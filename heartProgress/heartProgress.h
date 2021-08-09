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
	//���ƺ�����ʽ
	void paintEvent(QPaintEvent *event);
	//�����м䱳��
	void paintBg(QPainter* painter);
	//���ƽ�������
	void paintText(QPainter* painter);

private:
    Ui::heartProgressClass ui;
};
