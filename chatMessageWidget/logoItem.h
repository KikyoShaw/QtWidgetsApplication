#pragma once

#include <QWidget>
#include "ui_logoItem.h"

class LogoItem : public QWidget
{
	Q_OBJECT
public:
	LogoItem(QWidget *parent = Q_NULLPTR);
	~LogoItem();

	void setLogo(const QString& path);

private:
	Ui::logoitem ui;
};
