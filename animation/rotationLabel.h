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
	//ͼƬ��Դ
	QString m_imagePath;
	//��ת�Ƕ�
	int m_matrixNum;
	//������ת�Ƕȶ���
	QVariantAnimation *m_vAnimation = Q_NULLPTR;
};
