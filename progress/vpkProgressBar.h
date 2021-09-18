#pragma once

#include <QProgressBar>
#include <QVariant>

class QVariantAnimation;

class vpkProgressBar : public QProgressBar
{
	Q_OBJECT

public:
	vpkProgressBar(QWidget *parent = Q_NULLPTR);
	~vpkProgressBar();

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
