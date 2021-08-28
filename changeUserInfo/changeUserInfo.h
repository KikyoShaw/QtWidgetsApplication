#pragma once

#include <QtWidgets/QWidget>
#include "ui_changeUserInfo.h"

class changeUserInfo : public QWidget
{
    Q_OBJECT

public:
    changeUserInfo(QWidget *parent = Q_NULLPTR);

private:
    Ui::changeUserInfoClass ui;
};
