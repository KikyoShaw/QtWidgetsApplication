#include "smoothscrollbar.h"
#include <QWheelEvent>

SmoothScrollBar::SmoothScrollBar(QWidget* parent):QScrollBar(parent)
{
	//设置样式
	auto path = ":/qss/qrc/qss/blackScrollbar.qss";
	QFile QSS(path);
	if (QSS.open(QFile::ReadOnly)) {
		QString style(QSS.readAll());
		this->setStyleSheet(style);
	}

	//设置滑动条动画
    m_scrollAni=new QPropertyAnimation;
    m_scrollAni->setTargetObject(this);
    m_scrollAni->setPropertyName("value");
    m_scrollAni->setEasingCurve(QEasingCurve::OutQuint);
    m_scrollAni->setDuration(1000);
	m_targetValue =value();
}

void SmoothScrollBar::setValue(int value)
{
    m_scrollAni->stop();
    m_scrollAni->setStartValue(this->value());
    m_scrollAni->setEndValue(value);
    m_scrollAni->start();
}

void SmoothScrollBar::scroll(int value)
{
	m_targetValue -= value;
    setValue(m_targetValue);
}

void SmoothScrollBar::mousePressEvent(QMouseEvent *e)
{
    m_scrollAni->stop();
    QScrollBar::mousePressEvent(e);
	m_targetValue = value();

}

void SmoothScrollBar::mouseReleaseEvent(QMouseEvent *e)
{
    m_scrollAni->stop();
    QScrollBar::mouseReleaseEvent(e);
	m_targetValue = value();
}

void SmoothScrollBar::mouseMoveEvent(QMouseEvent *e)
{
    m_scrollAni->stop();
    QScrollBar::mouseMoveEvent(e);
	m_targetValue = value();
}

