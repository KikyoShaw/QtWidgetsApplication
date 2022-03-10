#pragma once

#include <QWidget>

enum BackgroundStyle
{
	E_NONE = 0,
	E_DOT = 1,
};

class Verification : public QWidget
{
	Q_OBJECT

public:
	Verification(QWidget *parent = Q_NULLPTR);
	~Verification();

	//背景样式
	 void setBackgroundStyle(BackgroundStyle style) { m_bstyle = style;}
	 //获取当前随机验证码
	 QString getVerificationCode() { return m_verificationCode; }

private slots:
	void sltTimerOut();

private:
	//获取随机验证码
	QString getVerificationCodeByRand();
	//获取颜色
	Qt::GlobalColor* getColors();

private:
	virtual void paintEvent(QPaintEvent *event);
	void paintDot(QPainter *painter);

	virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:
	QString m_verificationCode; //验证码字符串
	int m_codeNum; //验证码字符串个数
	Qt::GlobalColor *m_colors = nullptr; //颜色样式,因为随机颜色可能会导致字符显示不清，因此建议选定一些颜色放到数组里随机选择
	BackgroundStyle m_bstyle = BackgroundStyle::E_NONE; //绘制背景样式
	QTimer *m_updateTimer = nullptr; //定时器刷新（目前先不用，暂测试）
};
