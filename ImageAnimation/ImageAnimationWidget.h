#pragma once

#include <QWidget>

class QPropertyAnimation;

class ImageAnimationWidget : public QWidget
{
	Q_OBJECT
		Q_ENUMS(AnimationType)
		Q_PROPERTY(float factor READ getFactor WRITE setFactor)
		Q_PROPERTY(QString imageName1 READ getImageName1 WRITE setImageName1)
		Q_PROPERTY(QString imageName2 READ getImageName2 WRITE setImageName2)
		Q_PROPERTY(QPixmap pixmap1 READ getPixmap1 WRITE setPixmap1)
		Q_PROPERTY(QPixmap pixmap2 READ getPixmap2 WRITE setPixmap2)
		Q_PROPERTY(AnimationType animationType READ getAnimationType WRITE setAnimationType)

public:
	enum AnimationType {
		FadeEffect = 0,             //图像1渐渐变淡,图像2渐渐显现
		BlindsEffect = 1,           //百叶窗效果
		FlipRightToLeft = 2,        //图像从右向左翻转
		OutsideToInside = 3,        //从外到内水平分割
		MoveLeftToRightEffect = 4,  //图像1从左至右退出可视区域,同时图像2从左至右进入可视区域
		MoveRightToLeftEffect = 5,  //图像1从左至右退出可视区域,同时图像2从左至右进入可视区域
		MoveBottomToUpEffect = 6,   //图像1从下至上退出可视区域,同时图像2从下至上进入可视区域
		MoveUpToBottomEffect = 7,   //图像1从上至下退出可视区域,同时图像2从上至下进入可视区域
		MoveBottomToLeftUpEffect = 8//图像1不动,同时图像2从右下到左上
	};

	explicit ImageAnimationWidget(QWidget *parent = Q_NULLPTR);
	~ImageAnimationWidget();

protected:
	void paintEvent(QPaintEvent *);
	void fadeEffect(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2);
	void blindsEffect(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2);
	void flipRightToLeft(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2);
	void outsideToInside(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2);
	void moveLeftToRightEffect(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2);
	void moveRightToLeftEffect(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2);
	void moveBottomToUpEffect(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2);
	void moveUpToBottomEffect(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2);
	void moveBottomToLeftUpEffect(QPainter *painter, const QRect &rect, float factor, const QPixmap &pixmap1, const QPixmap &pixmap2);

private:
	float factor;                   //动画因子(0 - 1.0之间变化)
	QString imageName1;             //图片1路径名称
	QString imageName2;             //图片2路径名称
	QPixmap pixmap1;                //图片1
	QPixmap pixmap2;                //图片2
	AnimationType animationType;    //动画效果类型

	QPropertyAnimation *animation = nullptr;  //动画属性

public:
	float getFactor() { return factor; }
	QString getImageName1() { return imageName1; }
	QString getImageName2() { return imageName2;}
	QPixmap getPixmap1() { return pixmap1; }
	QPixmap getPixmap2() { return pixmap2; }
	AnimationType getAnimationType() { return animationType; }

	QSize sizeHint();
	QSize minimumSizeHint();

public slots:
	//设置动画因子
	void setFactor(float _factor) { factor = _factor; }

	//设置图片1+图片2路径名称
	void setImageName1(const QString &Name1) { imageName1 = Name1; }
	void setImageName2(const QString &Name2) { imageName2 = Name2; }

	//设置图片1+图片2
	void setPixmap1(const QPixmap &_pixmap1) { pixmap1 = _pixmap1; }
	void setPixmap2(const QPixmap &_pixmap2) { pixmap2 = _pixmap2; }

	//设置动画类型
	void setAnimationType(const AnimationType &_animationType) { animationType = _animationType; }

	//启动+停止动画
	void start();
	void stop();

};
