#pragma once

#include <QScrollBar>

/*
* 悬浮滑动条类
*/

class SuspensionScrollBar : public QScrollBar
{
	Q_OBJECT
public:
	explicit SuspensionScrollBar(Qt::Orientation type = Qt::Vertical, QWidget *parent = Q_NULLPTR);
	~SuspensionScrollBar();

	//设置滑动条样式
	void setScrollBarStyleSheet(const QString& style);

	//通过QSS文件直接设置滑动条样式
	void setScrollBarStyleSheetByQSS(const QString& qssPath);

public slots:
	void sltValueChanged(int value); //修改滑动条当前显示位置
	void sltRangeChanged(int min, int max); //修改滑动条显示尺寸
};

/*
* QListView
*/

#include <QListView>

class ScrollBarListView : public QListView
{
	Q_OBJECT
public:
	explicit ScrollBarListView(QWidget *parent = Q_NULLPTR);
	~ScrollBarListView();

	void setVerticalScrollBarVisible(bool visible); //是否显示垂直滑动条
	void setHorizontalScrollBarVisible(bool visible); //是否显示水平滑动条

private slots:
	void sltVerticalValueChanged(int value); //垂直滑动位置
	void sltHorizontalValueChanged(int value); //水平滑动位置

private:
	virtual void resizeEvent(QResizeEvent *event);

private:
	SuspensionScrollBar *m_verticalScrollBar = Q_NULLPTR; //垂直滑动条类
	SuspensionScrollBar *m_horizontalScrollBar = Q_NULLPTR; //水平滑动条类
};

/*
* QListWidget
*/

#include <QListWidget>

class ScrollBarListWidget : public QListWidget
{
	Q_OBJECT
public:
	explicit ScrollBarListWidget(QWidget *parent = Q_NULLPTR);
	~ScrollBarListWidget();

	void setVerticalScrollBarVisible(bool visible); //是否显示垂直滑动条
	void setHorizontalScrollBarVisible(bool visible); //是否显示水平滑动条

private slots:
	void sltVerticalValueChanged(int value); //垂直滑动位置
	void sltHorizontalValueChanged(int value); //水平滑动位置

private:
	virtual void resizeEvent(QResizeEvent *event);

private:
	SuspensionScrollBar *m_verticalScrollBar = Q_NULLPTR; //垂直滑动条类
	SuspensionScrollBar *m_horizontalScrollBar = Q_NULLPTR; //水平滑动条类
};

/*
* QScrollArea
*/
#include <QScrollArea>

class ScrollBarScrollArea : public QScrollArea
{
	Q_OBJECT
public:
	explicit ScrollBarScrollArea(QWidget *parent = Q_NULLPTR);
	~ScrollBarScrollArea();

	void setVerticalScrollBarVisible(bool visible); //是否显示垂直滑动条
	void setHorizontalScrollBarVisible(bool visible); //是否显示水平滑动条

private slots:
	void sltVerticalValueChanged(int value); //垂直滑动位置
	void sltHorizontalValueChanged(int value); //水平滑动位置

private:
	virtual void resizeEvent(QResizeEvent *event);

private:
	SuspensionScrollBar *m_verticalScrollBar = Q_NULLPTR; //垂直滑动条类
	SuspensionScrollBar *m_horizontalScrollBar = Q_NULLPTR; //水平滑动条类
};



