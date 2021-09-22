#pragma once

#include <QtWidgets/QWidget>
#include "ui_paintEvent.h"

class QPainter;
class QVariantAnimation;
class QPropertyAnimation;

class paintEvents : public QWidget
{
    Q_OBJECT

public:
	paintEvents(QWidget *parent = Q_NULLPTR);

private:
	//根据宽度得到文字
	QString getTextByWidth(const QFontMetrics& fm, const QString& text, int width);
	//根据文字宽高等到文字段落
	QStringList getTextLinesByRectSize(const QFontMetrics& fm, const QString& text, const QSize& size);

private slots:
	void sltAtIndexChanged(QVariant value);
	void sltValueChanged(QVariant value);

private:
	virtual void paintEvent(QPaintEvent *event);

	void paintHeart(QPainter *painter);
	void paintColorText(QPainter *painter);
	void paintColorText2(QPainter *painter);
	void paintColorText3(QPainter *painter);
	void paintColorText4(QPainter *painter);
	void paintAnimationText(QPainter *painter);
	void paintAnimationText2(QPainter *painter);
	void paintAnimationText3(QPainter *painter);
	void paintAnimationText4(QPainter *painter);

private:
    Ui::paintEventClass ui;
	//颜色进度
	double m_nAtIndex;
	//控制颜色进度动画
	QVariantAnimation *m_vAnimation = Q_NULLPTR;
	//透明度
	int m_nAlpha;
	//动画类
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
};
