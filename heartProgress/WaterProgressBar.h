#pragma once

#include <QProgressBar>

class QTimer;

class WaterProgressBar : public QProgressBar
{
	Q_OBJECT

public:
	WaterProgressBar(QWidget *parent);
	~WaterProgressBar();

protected:
	//ҳ���ػ��¼�
	void paintEvent(QPaintEvent *event);
private:
	void drawBackGround(QPainter* painter);
	void drawWaterWave(QPainter* painter);
	void drawText(QPainter* painter);

private:
	int m_iBorderWidth;//�߿���
	int m_iValue;//��ǰ����������
	double m_dOffset;//ˮ��ƫ����
	QColor m_waterColor;//ˮ����ɫ
	QColor m_backgroundColor;//������ɫ
	QColor m_borderColor;//�߿���ɫ
	QColor m_textColor;//�ı���ɫ
	QTimer *m_timer = nullptr;//����ˮ���ƶ��Ķ�ʱ��
};
