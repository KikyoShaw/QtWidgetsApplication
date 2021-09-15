#pragma once

#include <QLabel>
#include <QVariant>

class QVariantAnimation;

class rotationLabel : public QLabel
{
	Q_OBJECT
public:
	rotationLabel(QWidget *parent = Q_NULLPTR);
	~rotationLabel();

	void setImagePath(const QString& path);
	void startRotationAnimation();

private slots:
	void sltMatrixChanged(QVariant value);

private:
	void paintEvent(QPaintEvent *event);

private:
	//图片资源
	QString m_imagePath;
	//旋转角度
	int m_matrixNum;
	//控制旋转角度动画
	QVariantAnimation *m_vAnimation = Q_NULLPTR;
};
