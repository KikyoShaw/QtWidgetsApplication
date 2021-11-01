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

	//设置歌词控件长度
	void setLrcWidth(int w) { m_lrcWidth = w; };
	//设置歌词字体大小
	void setLrcTextFontSize(int size);
	//遮罩开启，参数是当前显示歌词开始到结束时间和歌词文本内容
	void startLrcMask(qint64 intervalTime, const QString& text);
	//关闭遮罩
	void stopLrcMask();

private slots:
	void sltTimerOut();

private:
	virtual void paintEvent(QPaintEvent *event);

private:
    Ui::LrcClass ui;
	//歌词的线性渐变填充
	QLinearGradient m_textLinearGradient;
	//遮罩的线性渐变填充
	QLinearGradient m_maskLinearGradient;
	//遮罩长度
	int m_lrcMaskWidth;
	//遮罩每次增加的长度
	int m_lrcMaskWidthInterval;
	//歌词控件长度
	int m_lrcWidth;
	//字体属性
	QFont m_font;
	//定时器实时刷新
	QTimer *m_timer = Q_NULLPTR;
};
