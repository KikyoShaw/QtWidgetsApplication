#pragma once

#include <QtWidgets/QWidget>
#include "ui_progressTest.h"

class progressTest : public QWidget
{
    Q_OBJECT

public:
    progressTest(QWidget *parent = Q_NULLPTR);
	~progressTest();

private:
    Ui::progressTestClass ui;
};
