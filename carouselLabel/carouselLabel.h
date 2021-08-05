#pragma once

#include <QLabel>
#include <QPushButton>
#include <QTimer>

class QButtonGroup;
class QHBoxLayout;
class QPropertyAnimation;
class QSequentialAnimationGroup;
class QParallelAnimationGroup;

class CarouselLabel : public QLabel
{
	Q_OBJECT
public:
	CarouselLabel(QWidget *parent = Q_NULLPTR);
	~CarouselLabel();

	void addImage(const QList<std::pair<QString, QString>>& imagepathpairlst);

private:
	void initControl();

private slots:
	void sltbuttonClicked(int);
	void sltImagevalueChanged(const QVariant & variant);
	void sltBtnExpvalueChanged(const QVariant & variant);
	void sltBtnShrikvalueChanged(const QVariant & variant);
	void sltImageShowTimeOut();

private:
	void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	int m_offset;
	int m_curIndex;
	int m_preIndex;
	bool m_blefttoright;
	QButtonGroup *m_btnGroup = nullptr;
	QParallelAnimationGroup *m_btnParalGroup = nullptr;
	QPropertyAnimation *m_imageAnimation = nullptr;
	QPropertyAnimation *m_btnShrikAnimation = nullptr;
	QPropertyAnimation *m_btnExpAnimation = nullptr;
	QWidget *m_horizontalLayoutWidget = nullptr;
	QHBoxLayout *m_switchBtnLayout = nullptr;
	QTimer m_imageTimer;
	QTimer m_sleepTimer;
	bool m_sleepFinsh;
	QList<std::pair<QString, QString>> m_imagepathpairlst;
};