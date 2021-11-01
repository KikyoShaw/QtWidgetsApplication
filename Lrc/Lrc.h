#pragma once

#include <QLabel>
#include <QLinearGradient>
#include "ui_Lrc.h"

class QTimer;

class Lrc : public QLabel
{
    Q_OBJECT

public:
    Lrc(QWidget *parent = Q_NULLPTR);
	~Lrc();

	//���ø�ʿؼ�����
	void setLrcWidth(int w) { m_lrcWidth = w; };
	//���ø�������С
	void setLrcTextFontSize(int size);
	//���ֿ����������ǵ�ǰ��ʾ��ʿ�ʼ������ʱ��͸���ı�����
	void startLrcMask(qint64 intervalTime, const QString& text);
	//�ر�����
	void stopLrcMask();

private slots:
	void sltTimerOut();

private:
	virtual void paintEvent(QPaintEvent *event);

private:
    Ui::LrcClass ui;
	//��ʵ����Խ������
	QLinearGradient m_textLinearGradient;
	//���ֵ����Խ������
	QLinearGradient m_maskLinearGradient;
	//���ֳ���
	int m_lrcMaskWidth;
	//����ÿ�����ӵĳ���
	int m_lrcMaskWidthInterval;
	//��ʿؼ�����
	int m_lrcWidth;
	//��������
	QFont m_font;
	//��ʱ��ʵʱˢ��
	QTimer *m_timer = Q_NULLPTR;
};
