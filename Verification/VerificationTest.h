#pragma once

#include <QWidget>
#include "ui_VerificationTest.h"

class VerificationTest : public QWidget
{
    Q_OBJECT

public:
	VerificationTest(QWidget *parent = Q_NULLPTR);
	~VerificationTest();

private slots:
	void sltVerficationCodeBtn();

private:
    Ui::VerificationClass ui;
};
