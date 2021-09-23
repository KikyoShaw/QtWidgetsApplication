#pragma once

#include <QWidget>

class QPainter;


enum DateProperty
{
	E_Property_None, //通用
	E_Property_Day = 1, //日期控件
	E_Property_Month = 2, //月份控件
	E_Property_Year = 3, //年份控件
};

enum ScrollType 
{
	E_VERTICAL = 1, //垂直显示
	E_HORIZONTAL = 2, //水平显示
};

class RollingBox : public QWidget
{
	Q_OBJECT

public:
	explicit RollingBox(QWidget *parent = 0);
	~RollingBox();
	//获取中间值
	inline int getValue() { return m_nCurrentValue; }
	//设置中间值
	inline void setValue(int nValue) { m_nCurrentValue = nValue; }
	//设置滚动的步长
	inline void setStep(int nStep) { m_nStep = nStep; }
	//设置显示个数
	inline void setDevice(int nDevice) { m_nDevice = nDevice; }
	//设置显示边界
	void setRang(int nMin, int nMax);
	//设置属性
	void setPropertys(DateProperty date) { m_dateProperty = date; };
	//水平还是垂直滑动
	void setScrollType(ScrollType types) { m_scrollType = types; };

protected:
	void wheelEvent(QWheelEvent* event);
	void paintEvent(QPaintEvent* event);
	//绘制中间选择
	void paintLine(QPainter* pPainter);
	//绘制文字
	void paintText(QPainter* pPainter, int nValue, int nOffSet, int nFontSize);

	//获取文案
	QString getTextByProperty(const QString &text);

	//获取样式
	QColor getTextStyle(int nOffSet);

signals:
	void sigCurrentValueChange(int nValue, DateProperty dateProperty);

private:
	//当前选中值
	int m_nCurrentValue;
	//偏离值
	int m_nOffSet;
	//滚动的最大值
	int m_nMax;
	//滚动的最小值
	int m_nMin;
	//显示的数量
	int m_nDevice;
	//滚动的步长
	int m_nStep;
	//属性
	DateProperty m_dateProperty;
	//水平/垂直滑动
	ScrollType m_scrollType;
};