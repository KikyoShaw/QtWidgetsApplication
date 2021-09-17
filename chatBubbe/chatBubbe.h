#pragma once

#include <QtWidgets/QWidget>
#include "ui_chatBubbe.h"

class chatBubbe : public QWidget
{
    Q_OBJECT

public:
    chatBubbe(QWidget *parent = Q_NULLPTR);
	~chatBubbe();

private:
    Ui::chatBubbeClass ui;
};
