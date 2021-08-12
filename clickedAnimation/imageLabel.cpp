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

	// �����
	painter.setRenderHints(QPainter::Antialiasing, true);
	// ͼƬƽ������
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	//����ͼƬ��Դ
	QPixmap disc(m_imagePath);
	//�趨��ת���ĵ�
	painter.translate(22, 24);
	//��ת�ĽǶ�
	painter.rotate(m_matrixNum);
	//�ָ����ĵ�
	painter.translate(-22, -24);
	//��ͼ����
	painter.drawPixmap(0, 0, 44, 48, disc);
}
