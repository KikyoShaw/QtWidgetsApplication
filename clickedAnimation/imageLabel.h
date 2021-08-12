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
	//ͼƬ��Դ
	QString m_imagePath;
	//��ת�Ƕ�
	int m_matrixNum;
};
