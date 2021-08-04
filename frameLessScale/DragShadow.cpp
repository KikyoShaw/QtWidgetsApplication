#include "DragShadow.h"

DragShadow::DragShadow(QWidget *parent) :
	QWidget(NULL)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
}

void DragShadow::setSizePos(int x, int y, int w, int h)
{
	if (w % 2 == 0)
		w += 1;
	if (h % 2 == 0)
		h += 1;
	this->setGeometry(x, y, w, h);
}

void DragShadow::setPos(int x, int y)
{
	this->move(x, y);
	this->update();
}

void DragShadow::setPos(QPoint pos)
{
	this->move(pos);
	this->update();
}

void DragShadow::showEvent(QShowEvent * event)
{

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
	m_image = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
#else
	QScreen *screen = QGuiApplication::primaryScreen();
	m_image = screen->grabWindow(0).toImage();
#endif
}

void DragShadow::paintEvent(QPaintEvent *)
{
	int LineCount = 4;
	QColor color;
	QPainter painter(this);
	painter.setBrush(Qt::NoBrush);
	QPen pen(Qt::SolidLine);
	pen.setColor(Qt::black);
	pen.setWidthF(1);
	painter.setPen(pen);
	painter.drawPoint(0, 0);


	for (int current = 0; current < LineCount; current++)
	{
		for (int i = current; i < (this->width() - current); i += 2) //x
		{
			this->getInvertColor(this->x() + i, this->y() + current, color);
			pen.setColor(color);
			painter.setPen(pen);
			painter.drawPoint(i, current);					    //draw top
			this->getInvertColor(i + this->x(), this->height() - current - 1 + this->y(), color);
			pen.setColor(color);
			painter.setPen(pen);
			painter.drawPoint(i, this->height() - current - 1); //draw bottom
		}
		for (int i = current; i < (this->height() - current); i += 2) //y
		{
			this->getInvertColor(current + this->x(), i + this->y(), color);
			pen.setColor(color);
			painter.setPen(pen);
			painter.drawPoint(current, i);					//draw left
			this->getInvertColor(this->width() - current - 1 + this->x(), i + this->y(), color);
			pen.setColor(color);
			painter.setPen(pen);
			painter.drawPoint(this->width() - current - 1, i); //draw right
		}
	}
}

bool DragShadow::getInvertColor(int x, int y, QColor &color)
{
	int ret = m_image.valid(x, y);
	//int ret = 0;
	if (ret)
	{
		QRgb rgb = m_image.pixel(x, y);
		color.setRgb(rgb);
		color.setRed(255 - color.red());
		color.setBlue(255 - color.blue());
		color.setGreen(255 - color.green());
	}
	else
	{
		color.setRed(0);
		color.setBlue(0);
		color.setGreen(0);
	}
	return ret;
}