#include "vTextEdits.h"

vTextEdits::vTextEdits(QWidget *parent /* = Q_NULLPTR */)
	:QTextEdit(parent)
{
}

vTextEdits::~vTextEdits()
{
}

void vTextEdits::focusInEvent(QFocusEvent * event)
{
	this->setStyleSheet("color: rgba(255,255,255,1);");
	this->
	QTextEdit::focusInEvent(event);
	emit sigFocusState(true);
}

void vTextEdits::focusOutEvent(QFocusEvent * event)
{
	this->setStyleSheet("color: rgba(255,255,255,0.5);");
	QTextEdit::focusOutEvent(event);
	emit sigFocusState(false);
}
