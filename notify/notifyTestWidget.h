#pragma once

#include <QWidget>
#include "ui_notifyTestWidget.h"

class NotifyManager;
class notifyTestWidget : public QWidget
{
	Q_OBJECT

public:
	notifyTestWidget(QWidget *parent = nullptr);
	~notifyTestWidget();

private:
	Ui::notifyTestWidgetClass ui;
	NotifyManager* manager = nullptr;
};
