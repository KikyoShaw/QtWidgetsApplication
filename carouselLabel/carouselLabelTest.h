#pragma once

#include <QtWidgets/QWidget>
#include "ui_carouselLabelTest.h"

class carouselLabelTest : public QWidget
{
    Q_OBJECT

public:
    carouselLabelTest(QWidget *parent = Q_NULLPTR);
	~carouselLabelTest();

private:
    Ui::carouselLabelClass ui;
};
