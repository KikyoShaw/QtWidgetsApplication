#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SmoothScrollBarTest.h"

class SmoothScrollBarTest : public QMainWindow
{
    Q_OBJECT

public:
    SmoothScrollBarTest(QWidget *parent = nullptr);
    ~SmoothScrollBarTest();

private:
    Ui::SmoothScrollBarTestClass ui;
};
