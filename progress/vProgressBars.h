#pragma once

#include <QProgressBar>
#include <QVariant>

class QVariantAnimation;

class vProgressBars : public QProgressBar
{
	Q_OBJECT

public:
	vProgressBars(QWidget *parent = Q_NULLPTR);
	~vProgressBars();

private slots:
	void sltAtIndexChanged(QVariant value);

protected:
	void paintEvent(QPaintEvent *event);

private:
	//��ɫ����
	double m_nAtIndex;
	//������ɫ���ȶ���
	QVariantAnimation *m_vAnimation = Q_NULLPTR;

};
