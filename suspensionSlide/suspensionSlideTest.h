#pragma once

#include <QtWidgets/QWidget>
#include "ui_suspensionSlideTest.h"

class suspensionSlideTest : public QWidget
{
    Q_OBJECT

public:
    suspensionSlideTest(QWidget *parent = Q_NULLPTR);

private:
    Ui::suspensionSlideTestClass ui;
};
