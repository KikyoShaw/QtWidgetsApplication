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
	//颜色进度
	double m_nAtIndex;
	//控制颜色进度动画
	QVariantAnimation *m_vAnimation = Q_NULLPTR;

};
