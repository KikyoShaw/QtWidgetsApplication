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
	//���ݿ�ȵõ�����
	QString getTextByWidth(const QFontMetrics& fm, const QString& text, int width);
	//�������ֿ�ߵȵ����ֶ���
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
	//��ɫ����
	double m_nAtIndex;
	//������ɫ���ȶ���
	QVariantAnimation *m_vAnimation = Q_NULLPTR;
	//͸����
	int m_nAlpha;
	//������
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
};
