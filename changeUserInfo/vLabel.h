#pragma once

#include <QLabel>

class vLabel : public QLabel
{
	Q_OBJECT
public:
	vLabel(QWidget *parent = Q_NULLPTR);
	~vLabel();

signals:
	void sigRightMouseClicked();
	void sigFocusOut();

private:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void focusInEvent(QFocusEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);
};

