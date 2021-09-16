#pragma once

#include <QtWidgets/QWidget>
#include "ui_paintEvent.h"

class paintEvents : public QWidget
{
    Q_OBJECT

public:
	paintEvents(QWidget *parent = Q_NULLPTR);

private:
	virtual void paintEvent(QPaintEvent *event);

private:
    Ui::paintEventClass ui;
};
