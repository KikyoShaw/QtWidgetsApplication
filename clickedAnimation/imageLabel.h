#pragma once

#include <QLabel>

class ImageLabel : public QLabel
{
	Q_OBJECT
public:
	ImageLabel(QWidget *parent = Q_NULLPTR);
	~ImageLabel();

	void setImagePath(const QString& path);
	void setMatrix(int radius);

private:
	void paintEvent(QPaintEvent *);

private:
	//图片资源
	QString m_imagePath;
	//旋转角度
	int m_matrixNum;
};
