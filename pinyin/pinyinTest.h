#pragma once

#include <QtWidgets/QWidget>
#include "ui_pinyinTest.h"

class pinyinTest : public QWidget
{
    Q_OBJECT

public:
    pinyinTest(QWidget *parent = Q_NULLPTR);
	~pinyinTest();

private slots:
	void sltTextChanged(const QString& text);

private:
    Ui::pinyinTestClass ui;
};
