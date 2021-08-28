#pragma once

#include <QLineEdit>

class vLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	vLineEdit(QWidget *parent = Q_NULLPTR);
	~vLineEdit();

private:
	virtual void focusInEvent(QFocusEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);
};

