#pragma once

#include <QtWidgets/QWidget>
#include "ui_ImageAnimationTest.h"

class ImageAnimationTest : public QWidget
{
    Q_OBJECT

public:
    ImageAnimationTest(QWidget *parent = Q_NULLPTR);
	~ImageAnimationTest();

private:
    Ui::ImageAnimationTestClass ui;
};
