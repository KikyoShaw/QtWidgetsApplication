#pragma once

#include <QtWidgets/QWidget>
#include "ui_JsWithCpp.h"

class JsWithCpp : public QWidget
{
    Q_OBJECT

public:
    JsWithCpp(QWidget *parent = Q_NULLPTR);
	~JsWithCpp();

private:
	void loadWeb();

private slots:
	void parseWebMessage(const QString & message);
	void sendMessageToWeb();

private:
    Ui::JsWithCppClass ui;
};
