#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DockWin.h"

class DockWin : public QMainWindow
{
    Q_OBJECT

public:
    DockWin(QWidget *parent = Q_NULLPTR);

private:
    Ui::DockWinClass ui;
};
