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
		FadeEffect = 0,             //ͼ��1�����䵭,ͼ��2��������
		BlindsEffect = 1,           //��Ҷ��Ч��
		FlipRightToLeft = 2,        //ͼ���������ת
		OutsideToInside = 3,        //���⵽��ˮƽ�ָ�
		MoveLeftToRightEffect = 4,  //ͼ��1���������˳���������,ͬʱͼ��2�������ҽ����������
		MoveRightToLeftEffect = 5,  //ͼ��1���������˳���������,ͬʱͼ��2�������ҽ����������
		MoveBottomToUpEffect = 6,   //ͼ��1���������˳���������,ͬʱͼ��2�������Ͻ����������
		MoveUpToBottomEffect = 7,   //ͼ��1���������˳���������,ͬʱͼ��2�������½����������
		MoveBottomToLeftUpEffect = 8//ͼ��1����,ͬʱͼ��2�����µ�����
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
	float factor;                   //��������(0 - 1.0֮��仯)
	QString imageName1;             //ͼƬ1·������
	QString imageName2;             //ͼƬ2·������
	QPixmap pixmap1;                //ͼƬ1
	QPixmap pixmap2;                //ͼƬ2
	AnimationType animationType;    //����Ч������

	QPropertyAnimation *animation = nullptr;  //��������

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
	//���ö�������
	void setFactor(float _factor) { factor = _factor; }

	//����ͼƬ1+ͼƬ2·������
	void setImageName1(const QString &Name1) { imageName1 = Name1; }
	void setImageName2(const QString &Name2) { imageName2 = Name2; }

	//����ͼƬ1+ͼƬ2
	void setPixmap1(const QPixmap &_pixmap1) { pixmap1 = _pixmap1; }
	void setPixmap2(const QPixmap &_pixmap2) { pixmap2 = _pixmap2; }

	//���ö�������
	void setAnimationType(const AnimationType &_animationType) { animationType = _animationType; }

	//����+ֹͣ����
	void start();
	void stop();

};
