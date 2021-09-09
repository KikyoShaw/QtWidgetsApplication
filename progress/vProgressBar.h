#pragma once

#include <QProgressBar>

class vProgressBar : public QProgressBar
{
	Q_OBJECT

public:
	vProgressBar(QWidget *parent = Q_NULLPTR);
	~vProgressBar();

protected:
	void paintEvent(QPaintEvent *event);

};
