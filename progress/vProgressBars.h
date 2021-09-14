#pragma once

#include <QProgressBar>

class vProgressBars : public QProgressBar
{
	Q_OBJECT

public:
	vProgressBars(QWidget *parent = Q_NULLPTR);
	~vProgressBars();

protected:
	void paintEvent(QPaintEvent *event);

};
