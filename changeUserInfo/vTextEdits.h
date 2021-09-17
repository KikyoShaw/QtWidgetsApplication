#pragma once

#include <QTextEdit>

class vTextEdits : public QTextEdit
{
	Q_OBJECT
public:
	vTextEdits(QWidget *parent = Q_NULLPTR);
	~vTextEdits();

signals:
	void sigFocusState(bool state);

private:
	virtual void focusInEvent(QFocusEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);
};

