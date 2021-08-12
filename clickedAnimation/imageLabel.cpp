#include "imageLabel.h"
#include <QPainter>

ImageLabel::ImageLabel(QWidget *parent /* = Q_NULLPTR */)
	:QLabel(parent), m_matrixNum(0), m_imagePath(QString())
{

}

ImageLabel::~ImageLabel()
{
}

void ImageLabel::setImagePath(const QString & path)
{
	m_imagePath = path;
}

void ImageLabel::setMatrix(int radius)
{
	m_matrixNum = radius;
	update();
}

void ImageLabel::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 图片平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	//加载图片资源
	QPixmap disc(m_imagePath);
	//设定旋转中心点
	painter.translate(22, 24);
	//旋转的角度
	painter.rotate(m_matrixNum);
	//恢复中心点
	painter.translate(-22, -24);
	//画图操作
	painter.drawPixmap(0, 0, 44, 48, disc);
}
