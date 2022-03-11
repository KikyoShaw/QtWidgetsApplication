#pragma once

#include <QtWidgets/QWidget>
#include "ui_IconPanelTest.h"

class IconPanel : public QWidget
{
    Q_OBJECT

public:
    IconPanel(QWidget *parent = Q_NULLPTR);

private:
	void initPanel1();
	void initPanel2();
	void initPanel3();
	void initPanel4();
	bool checkIcon(int icon);

private slots:
	void sltToggled1(bool check);
	void sltToggled2(bool check);
	void sltToggled3(bool check);
	void sltToggled4(bool check);

private:
    Ui::IconPanelClass ui;

	QList<QWidget *> labs1;
	QList<QWidget *> labs2;
	QList<QWidget *> labs3;
	QList<QWidget *> labs4;
};
