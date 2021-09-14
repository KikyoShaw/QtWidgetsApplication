#pragma once

#include <QWidget>
#include <QProgressBar>

class QPropertyAnimation;

class AnimationProcessBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit AnimationProcessBar(QWidget *parent = nullptr);
	~AnimationProcessBar();

private slots:
	void sltValueChanged(const QVariant &value);

protected:
    void paintEvent(QPaintEvent *event);

private:
	//透明度
    int m_nAlpha;

	//动画类
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
};
