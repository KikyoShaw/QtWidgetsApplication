#pragma once

#include <QLabel>

class vLabel : public QLabel
{
	Q_OBJECT
public:
	vLabel(QWidget *parent = Q_NULLPTR);
	~vLabel();

	void setFocusSignals(bool isOpen) { m_isOpen = isOpen; };

signals:
	void sigRightMouseClicked();
	void sigFocusState(bool state);

private:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void focusInEvent(QFocusEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);

private:
	bool m_isOpen;
};

