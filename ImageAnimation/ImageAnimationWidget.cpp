#include "ImageAnimationWidget.h"
#include <QPainter>

ImageAnimationWidget::ImageAnimationWidget(QWidget *parent)
	: QWidget(parent)
{
	
}

ImageAnimationWidget::~ImageAnimationWidget()
{
}

void ImageAnimationWidget::paintEvent(QPaintEvent *event)
{
	if (pixmap1.isNull() || pixmap2.isNull()) {
		return;
	}

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	switch (animationType) {
	case 0:
		fadeEffect(&painter, geometry(), factor, pixmap1, pixmap2);
		break;
	case 1:
		blindsEffect(&painter, geometry(), factor, pixmap1, pixmap2);
		break;
	case 2:
		flipRightToLeft(&painter, geometry(), factor, pixmap1, pixmap2);
		break;
	case 3:
		outsideToInside(&painter, geometry(), factor, pixmap1, pixmap2);
		break;
	case 4:
		moveLeftToRightEffect(&painter, geometry(), factor, pixmap1, pixmap2);
		break;
	case 5:
		moveRightToLeftEffect(&painter, geometry(), factor, pixmap1, pixmap2);
		break;
	case 6:
		moveBottomToUpEffect(&painter, geometry(), factor, pixmap1, pixmap2);
		break;
	case 7:
		moveUpToBottomEffect(&painter, geometry(), factor, pixmap1, pixmap2);
		break;
	case 8:
		moveBottomToLeftUpEffect(&painter, geometry(), factor, pixmap1, pixmap2);
		break;
	default:
		break;
	}
}

void ImageAnimationWidget::fadeEffect(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2)
{
	int w = rect.width();
	int h = rect.height();
	int alpha = 255 * (1.0f - factor);

	QPixmap alphaPixmap(pixmap1.size());
	alphaPixmap.fill(Qt::transparent);

	QPainter p1(&alphaPixmap);
	p1.setCompositionMode(QPainter::CompositionMode_Source);
	p1.drawPixmap(0, 0, pixmap1);
	p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	p1.fillRect(alphaPixmap.rect(), QColor(0, 0, 0, alpha));
	p1.end();

	int x = (w - pixmap1.width()) / 2;
	int y = (h - pixmap1.height()) / 2;
	painter->drawPixmap(x, y, alphaPixmap);

	alpha = 255 * (factor);
	alphaPixmap.fill(Qt::transparent);
	QPainter p2(&alphaPixmap);
	p2.setCompositionMode(QPainter::CompositionMode_Source);
	p2.drawPixmap(0, 0, pixmap2);
	p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	p2.fillRect(alphaPixmap.rect(), QColor(0, 0, 0, alpha));
	p2.end();

	painter->drawPixmap(x, y, alphaPixmap);
}

void ImageAnimationWidget::blindsEffect(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2)
{
	int i, n, w, h, x1, y1, x2, y2, dh, ddh;

	w = rect.width();
	h = rect.height();
	x1 = (w - pixmap1.width()) / 2;
	y1 = (h - pixmap1.height()) / 2;
	x2 = (w - pixmap2.width()) / 2;
	y2 = (h - pixmap2.height()) / 2;

	painter->drawPixmap(x1, y1, pixmap1);

	n = 10;
	dh = pixmap2.height() / n;
	ddh = factor * dh;
	if (ddh < 1) {
		ddh = 1;
	}

	for (i = 0; i < n; i++) {
		painter->drawPixmap(x2, y2 + i * dh, pixmap2, 0, i * dh, pixmap2.width(), ddh);
	}
}

void ImageAnimationWidget::flipRightToLeft(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2)
{
	int x1, y1, x2, y2, w, h;
	float rot;
	QTransform trans;

	w = rect.width();
	h = rect.height();
	x1 = (w - pixmap1.width()) / 2;
	y1 = (h - pixmap1.height()) / 2;
	x2 = (w - pixmap2.width()) / 2;
	y2 = (h - pixmap2.height()) / 2;

	rot = factor * 90.0f;
	trans.translate(w * (1 - factor), h / 2);
	trans.rotate(rot, Qt::YAxis);
	trans.translate(-w, -h / 2);

	painter->setTransform(trans);
	painter->drawPixmap(x1, y1, pixmap1);
	painter->resetTransform();

	trans.reset();
	rot = 90 * (factor - 1);
	trans.translate(w * (1 - factor), h / 2);
	trans.rotate(rot, Qt::YAxis);
	trans.translate(0, -h / 2);

	painter->setTransform(trans);
	painter->drawPixmap(x2, y2, pixmap2);
	painter->resetTransform();
}

void ImageAnimationWidget::outsideToInside(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2)
{
	int   w, h, x1, y1, x2, y2, x3, y3, dh, ddh;

	w = rect.width();
	h = rect.height();
	x1 = (w - pixmap1.width()) / 2;
	y1 = (h - pixmap1.height()) / 2;
	painter->drawPixmap(x1, y1, pixmap1);

	dh = pixmap2.height() / 2;
	ddh = factor * dh;
	if (ddh < 1) {
		ddh = 1;
	}

	x2 = (w - pixmap2.width()) / 2;
	y2 = (h - pixmap2.height()) / 2;
	painter->drawPixmap(x2, y2, pixmap2, 0, 0, pixmap2.width(), ddh);

	x3 = (w - pixmap2.width()) / 2;
	y3 = dh * (1.0f - factor) + h / 2;
	if (y3 != h / 2) {
		y3 += 1;
	}

	painter->drawPixmap(x3, y3, pixmap2, 0, pixmap2.height() - ddh, pixmap2.width(), ddh);
}

void ImageAnimationWidget::moveLeftToRightEffect(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2)
{
	int x, y, w, h, x1, y1, x2, y2;

	w = rect.width();
	h = rect.height();
	x1 = (w - pixmap1.width()) / 2;
	y1 = (h - pixmap1.height()) / 2;
	x2 = (w - pixmap2.width()) / 2;
	y2 = (h - pixmap2.height()) / 2;

	x = x1 + w * factor;
	y = y1;
	painter->drawPixmap(x, y, pixmap1);

	x = x2 + w * (factor - 1);
	y = y2;
	painter->drawPixmap(x, y, pixmap2);
}

void ImageAnimationWidget::moveRightToLeftEffect(QPainter * painter, const QRect & rect, float factor, const QPixmap & pixmap1, const QPixmap & pixmap2)
{
}

void ImageAnimationWidget::moveBottomToUpEffect(QPainter * painter, const QRect & rect, float factor, const QPixmap & pixmap1, const QPixmap & pixmap2)
{
}

void ImageAnimationWidget::moveUpToBottomEffect(QPainter * painter, const QRect & rect, float factor, const QPixmap & pixmap1, const QPixmap & pixmap2)
{
}

void ImageAnimationWidget::moveBottomToLeftUpEffect(QPainter * painter, const QRect & rect, float factor, const QPixmap & pixmap1, const QPixmap & pixmap2)
{
}

QSize ImageAnimationWidget::sizeHint()
{
	return QSize();
}

QSize ImageAnimationWidget::minimumSizeHint()
{
	return QSize();
}

void ImageAnimationWidget::start()
{
}

void ImageAnimationWidget::stop()
{
}
