#pragma once

#include <QtWidgets/QWidget>
#include "ui_moveWidgetTest.h"

class MoveWidget;

class moveWidgetTest: public QWidget
{
    Q_OBJECT

public:
	moveWidgetTest(QWidget *parent = Q_NULLPTR);
	~moveWidgetTest();

private:
    Ui::moveWidgetClass ui;
	MoveWidget *m_moveWidget = nullptr;
};
