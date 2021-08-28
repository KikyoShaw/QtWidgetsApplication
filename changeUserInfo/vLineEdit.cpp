#include "vLineEdit.h"

vLineEdit::vLineEdit(QWidget *parent /* = Q_NULLPTR */)
	:QLineEdit(parent)
{

}

vLineEdit::~vLineEdit()
{
}

void vLineEdit::focusInEvent(QFocusEvent * event)
{
	this->setStyleSheet("color: rgba(255,255,255,1);background-color:transparent;border: 1px solid #01EEC3;");
	QLineEdit::focusInEvent(event);
}

void vLineEdit::focusOutEvent(QFocusEvent * event)
{
	this->setStyleSheet("color: rgba(255,255,255,0.5);");
	QLineEdit::focusOutEvent(event);
}
