#pragma once

#include <QLabel>
#include <QTimer>

#define ORIGIN_X 5

class VMarqueeLabel : public QLabel
{
	Q_OBJECT

public:
	VMarqueeLabel(QWidget *parent = Q_NULLPTR);
	~VMarqueeLabel();
	void clear();
	void setText(const QString& text);

protected:
	void paintEvent(QPaintEvent *event);

private:
	QTimer m_Timer;
	int m_PosX = ORIGIN_X;
};
