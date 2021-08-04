#ifndef DRAGSHADOW_H
#define DRAGSHADOW_H
#include <QtGui>
#include <QWidget>

class DragShadow : public QWidget
{
	Q_OBJECT
private:
	QImage m_image;
protected:
	bool getInvertColor(int x, int y, QColor &color);
	void paintEvent(QPaintEvent *);
	void showEvent(QShowEvent * event);
public:
	explicit DragShadow(QWidget *parent = 0);
	void setSizePos(int x, int y, int w, int h);
	void setPos(int x, int y);
	void setPos(QPoint pos);
};
#endif // DRAGSHADOW_H