#pragma once

#include <QWidget>

class QPainter;

enum PercentStyle_Type 
{
	PercentStyle_Rect = 0,  //矩形
	PercentStyle_Circle,    //圆
	PercentStyle_Ellipse,   //椭圆

};

class vWaterProgressBar : public QWidget
{
	Q_OBJECT

public:
	explicit vWaterProgressBar(QWidget *parent = nullptr);
	~vWaterProgressBar();

	//进度条值
	void setValue(int value) { m_value = qMax(m_minValue, qMin(m_maxValue, value)); };
	int value() { return m_value; };

	//进度条最大值与最小值
	void setRange(int nMax, int nMin) { m_minValue = nMax; m_maxValue = nMin;};
	int maxValue() { return m_maxValue; };
	int minValue() { return m_minValue; };

	//进度条样式
	void setPercentStyle(PercentStyle_Type type) { m_percentStyle = type; };
	PercentStyle_Type percentStyle() { return m_percentStyle; };

	//波浪密度
	void setWaterDensity(int val) { m_waterDensity = val; };
	int waterDensity() { return m_waterDensity; };

	//波浪颜色
	void setColor(QColor col) { m_frontColor = col; };
	QColor color() { return m_frontColor; };

	//波浪尺寸
	void setWaterHeight(double val) { m_waterHeight = val; };
	double waterHeight() { return m_waterHeight; };

	//边框尺寸
	void setBorderWidth(int val) { m_borderWidth = val; };
	int borderWidth() { return m_borderWidth; };

	//文字颜色
	void setTextColor(QColor col) { m_textColor = col; };
	QColor textColor() { return m_textColor; };

	//边框颜色
	void setBoderColor(QColor col) { m_boderColor = col; };
	QColor boderColor() { return m_boderColor; };

	//背景颜色
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
