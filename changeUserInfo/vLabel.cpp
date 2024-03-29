#include "vLabel.h"
#include <QMouseEvent>

vLabel::vLabel(QWidget *parent /* = Q_NULLPTR */)
	:QLabel(parent)
{

}

vLabel::~vLabel()
{
}

void vLabel::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		emit sigRightMouseClicked();
	}
	this->setFocus();
	//QLabel::mousePressEvent(event);
}

void vLabel::focusInEvent(QFocusEvent * event)
{
	if (m_isOpen) {
		this->setStyleSheet("color: rgba(255,255,255,1);background-color:transparent;border: 1px solid #01EEC3;");
		emit sigFocusState(true);
	}
	QLabel::focusInEvent(event);
}

void vLabel::focusOutEvent(QFocusEvent * event)
{
	if (m_isOpen) {
		this->setStyleSheet("color: rgba(255,255,255,0.5);");
		emit sigFocusState(false);
	}
	QLabel::focusOutEvent(event);
}
