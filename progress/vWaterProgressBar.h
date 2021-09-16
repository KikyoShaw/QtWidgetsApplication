#pragma once

#include <QWidget>

class QPainter;

enum PercentStyle_Type 
{
	PercentStyle_Rect = 0,  //����
	PercentStyle_Circle,    //Բ
	PercentStyle_Ellipse,   //��Բ

};

class vWaterProgressBar : public QWidget
{
	Q_OBJECT

public:
	explicit vWaterProgressBar(QWidget *parent = nullptr);
	~vWaterProgressBar();

	//������ֵ
	void setValue(int value) { m_value = qMax(m_minValue, qMin(m_maxValue, value)); };
	int value() { return m_value; };

	//���������ֵ����Сֵ
	void setRange(int nMax, int nMin) { m_minValue = nMax; m_maxValue = nMin;};
	int maxValue() { return m_maxValue; };
	int minValue() { return m_minValue; };

	//��������ʽ
	void setPercentStyle(PercentStyle_Type type) { m_percentStyle = type; };
	PercentStyle_Type percentStyle() { return m_percentStyle; };

	//�����ܶ�
	void setWaterDensity(int val) { m_waterDensity = val; };
	int waterDensity() { return m_waterDensity; };

	//������ɫ
	void setColor(QColor col) { m_frontColor = col; };
	QColor color() { return m_frontColor; };

	//���˳ߴ�
	void setWaterHeight(double val) { m_waterHeight = val; };
	double waterHeight() { return m_waterHeight; };

	//�߿�ߴ�
	void setBorderWidth(int val) { m_borderWidth = val; };
	int borderWidth() { return m_borderWidth; };

	//������ɫ
	void setTextColor(QColor col) { m_textColor = col; };
	QColor textColor() { return m_textColor; };

	//�߿���ɫ
	void setBoderColor(QColor col) { m_boderColor = col; };
	QColor boderColor() { return m_boderColor; };

	//������ɫ
	void setBgColor(QColor col) { m_bgColor = col; };
	QColor bgColor() { return m_bgColor; };

 protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void timerEvent(QTimerEvent *event);

	void drawValue(QPainter *painter);

private:
   PercentStyle_Type m_percentStyle = PercentStyle_Circle;
   QColor m_frontColor = Qt::red;
   QColor m_textColor = Qt::white;
   QColor m_boderColor = QColor(255, 255, 255);
   QColor m_bgColor = QColor(255, 255, 255, 51);
   QFont m_font;
   int m_value = 50;
   int m_minValue = 0;
   int m_maxValue = 100;
   int m_waterDensity = 5;
   double m_waterHeight = 0.05;
   double m_offset = 50;
   int m_borderWidth = 0;
 };
