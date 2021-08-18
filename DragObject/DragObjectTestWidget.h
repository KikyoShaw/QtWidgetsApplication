#pragma once

#include <QtWidgets/QWidget>
#include "ui_DragObjectTestWidget.h"

class DragObjectTestWidget : public QWidget
{
    Q_OBJECT

public:
    DragObjectTestWidget(QWidget *parent = Q_NULLPTR);
	~DragObjectTestWidget();

private:
    Ui::DragObjectTestWidgetClass ui;
};
