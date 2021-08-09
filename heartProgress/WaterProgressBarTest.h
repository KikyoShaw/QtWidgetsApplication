#pragma once

#include <QWidget>
#include "ui_WaterProgressBarTest.h"

class WaterProgressBarTest : public QWidget
{
	Q_OBJECT
public:
	WaterProgressBarTest(QWidget * parent = Q_NULLPTR);
	~WaterProgressBarTest();

private:
	Ui::test ui;
};