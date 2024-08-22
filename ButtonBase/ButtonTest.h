#pragma once

#include <QtWidgets/QWidget>
#include "ui_ButtonTest.h"

class ButtonBase : public QWidget
{
    Q_OBJECT

public:
    ButtonBase(QWidget *parent = nullptr);
    ~ButtonBase();

private:
    Ui::ButtonBaseClass ui;
};
