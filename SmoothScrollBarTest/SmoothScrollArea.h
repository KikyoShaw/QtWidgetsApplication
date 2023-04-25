#pragma once

#include <QWidget>
#include <QScrollArea>
#include "smoothscrollbar.h"

class SmoothScrollArea : public QScrollArea
{
    Q_OBJECT

public:
    explicit SmoothScrollArea(QWidget *parent = nullptr);

	void InitSmoothScrollArea();

private:
    SmoothScrollBar* vScrollBar;
    void wheelEvent(QWheelEvent* e);
};
