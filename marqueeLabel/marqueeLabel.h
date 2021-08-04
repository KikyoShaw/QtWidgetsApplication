#pragma once

#include <QtWidgets/QWidget>
#include "ui_marqueeLabel.h"

class marqueeLabel : public QWidget
{
    Q_OBJECT

public:
    marqueeLabel(QWidget *parent = Q_NULLPTR);

private:
    Ui::marqueeLabelClass ui;
};
