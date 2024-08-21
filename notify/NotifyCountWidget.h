#pragma once

#include "arrangedWidget.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPropertyAnimation>

class NotifyCountWidget : public ArrangedWidget
{
	Q_OBJECT

public:
	NotifyCountWidget(NotifyManager* manager, QWidget *parent = nullptr);
	~NotifyCountWidget();
	void setCount(int count);

private:
	QLabel* iconLabel;
	QLabel* countLabel;
	QPropertyAnimation* flickerAnim;
};
