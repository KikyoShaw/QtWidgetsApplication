#ifndef QBANNERWIDGET_H
#define QBANNERWIDGET_H

#include <QList>
#include <QWidget>

class QParallelAnimationGroup;
class QPropertyAnimation;

enum emAnimationType {
    UnKnow = -1,
    Next = 0,
    Prev = 1,
    Jump,
};

class QBannerWidget : public QWidget
{
    Q_OBJECT

  public:
	  QBannerWidget(QWidget *parent);
    ~QBannerWidget();

  public:
    void addImage(const QString &image);
	void setImageSize(int w, int h);

  public slots:
    void sltNext();
    void sltPrev();
    void sltJump(int index);

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    void drawLeft(QPainter *painter);
    void drawCenter(QPainter *painter);
    void drawRight(QPainter *painter);
    QImage getGrayImage(const QImage &image);

  private slots:
    void slot_ValueChanged(const QVariant &value);
    void slot_finished();

  private:
    QStringList x_imageList;
    int x_index;
    bool x_isAnimation;
    QParallelAnimationGroup *x_pAnimationGroup = nullptr;
    QPropertyAnimation *x_pLeftAnimation = nullptr;
    QPropertyAnimation *x_pCenterAnimation = nullptr;
    QPropertyAnimation *x_pRightAnimation = nullptr;
    emAnimationType x_animationType;
    QRect x_leftRect, x_centerRect, x_rightRect;
};

#endif
