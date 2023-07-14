#include "vComboBox.h"
#include <QEvent>

vComboBox::vComboBox(QWidget *parent)
	: QComboBox(parent)
{
	this->installEventFilter(this);
}

vComboBox::~vComboBox()
{
}

void vComboBox::setPopupLocation(int x, int y)
{
	m_posX = x;
	m_posY = y;
}

void vComboBox::showPopup()
{
	emit sigPopup();
	QComboBox::showPopup();
	QWidget *popup = this->findChild<QFrame*>();
	popup->move(popup->x() + m_posX, popup->y() + m_posY);
}

void vComboBox::hidePopup()
{
	emit sigHidePopup();
	QComboBox::hidePopup();
}

bool vComboBox::eventFilter(QObject * obj, QEvent * event)
{
	if (obj == this) {
		if (event->type() == QEvent::Wheel) {
			return true;//禁用下拉框的滚轮改变项的功能
		}
	}
	return QComboBox::eventFilter(obj, event);
}
