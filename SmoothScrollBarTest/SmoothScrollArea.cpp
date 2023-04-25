#include "smoothScrollArea.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QWheelEvent>
#include <QDebug>

SmoothScrollArea::SmoothScrollArea(QWidget *parent) : QScrollArea(parent)
{
    vScrollBar = new SmoothScrollBar();
    vScrollBar->setOrientation(Qt::Orientation::Vertical);
    setVerticalScrollBar(vScrollBar);

}

void SmoothScrollArea::InitSmoothScrollArea()
{
	auto layout = new QVBoxLayout;
	QWidget* w = new QWidget;
	for (int i = 0; i < 100; i++) {
		QFont font;
		font.setPointSize(12);
		auto a = new QLabel(QString::number(i));
		a->setFont(font);
		layout->addWidget(a);
	}
	w->setLayout(layout);
	setWidget(w);
}

void SmoothScrollArea::wheelEvent(QWheelEvent *e)
{
    vScrollBar->scroll(e->angleDelta().y());
}
