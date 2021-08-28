#pragma once

#include <QWidget>
#include "ui_baseInfoWidget.h"

class BaseInfoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit BaseInfoWidget(QWidget *parent = Q_NULLPTR);
	~BaseInfoWidget();

private:
	Ui::baseInfo ui;
};