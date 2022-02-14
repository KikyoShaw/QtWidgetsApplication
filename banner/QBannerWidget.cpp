#include "QBannerWidget.h"
#include <QApplication>
#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPixmap>
#include <QPropertyAnimation>

QBannerWidget::QBannerWidget(QWidget *parent) : QWidget(parent)
{
    //setCursor(Qt::PointingHandCursor);

    x_isAnimation = false;
    x_index = 0;
    x_animationType = emAnimationType::UnKnow;

    x_pLeftAnimation = new QPropertyAnimation(this, "");
    x_pCenterAnimation = new QPropertyAnimation(this, "");
    x_pRightAnimation = new QPropertyAnimation(this, "");

    x_pAnimationGroup = new QParallelAnimationGroup(this);

    x_pLeftAnimation->setDuration(300);
    x_pCenterAnimation->setDuration(300);
    x_pRightAnimation->setDuration(300);

    x_pAnimationGroup->addAnimation(x_pLeftAnimation);
    x_pAnimationGroup->addAnimation(x_pCenterAnimation);
    x_pAnimationGroup->addAnimation(x_pRightAnimation);

    connect(x_pLeftAnimation, SIGNAL(valueChanged(const QVariant &)), this, SLOT(slot_ValueChanged(const QVariant &)));
    connect(x_pCenterAnimation, SIGNAL(valueChanged(const QVariant &)), this,
            SLOT(slot_ValueChanged(const QVariant &)));
    connect(x_pRightAnimation, SIGNAL(valueChanged(const QVariant &)), this, SLOT(slot_ValueChanged(const QVariant &)));
    connect(x_pCenterAnimation, SIGNAL(finished()), this, SLOT(slot_finished()));
}

QBannerWidget::~QBannerWidget() {}

void QBannerWidget::addImage(const QString &image)
{
    x_imageList.append(image);
}

void QBannerWidget::setImageSize(int w, int h)
{
	resize(w, h);
}

void QBannerWidget::sltNext()
{
    if (!x_isAnimation) {
        x_animationType = emAnimationType::Next;
        x_isAnimation = true;

        x_pLeftAnimation->setStartValue(QRect(115, 0, 540, 200));
        x_pLeftAnimation->setEndValue(QRect(0, 10, 503, 190));

        x_pCenterAnimation->setStartValue(QRect(267, 10, 503, 190));
        x_pCenterAnimation->setEndValue(QRect(115, 0, 540, 200));

        x_pRightAnimation->setStartValue(QRect(152, 10, 503, 190));
        x_pRightAnimation->setEndValue(QRect(267, 10, 503, 190));

        x_pLeftAnimation->start();
        x_pRightAnimation->start();
        x_pCenterAnimation->start();
    }
}

void QBannerWidget::sltPrev()
{
    if (!x_isAnimation) {
        x_animationType = emAnimationType::Prev;
        x_isAnimation = true;

        x_pLeftAnimation->setStartValue(QRect(115, 10, 503, 190));
        x_pLeftAnimation->setEndValue(QRect(0, 10, 503, 190));

        x_pCenterAnimation->setStartValue(QRect(0, 10, 503, 190));
        x_pCenterAnimation->setEndValue(QRect(115, 0, 540, 200));

        x_pRightAnimation->setStartValue(QRect(115, 0, 540, 200));
        x_pRightAnimation->setEndValue(QRect(267, 10, 503, 190));

        x_pLeftAnimation->start();
        x_pRightAnimation->start();
        x_pCenterAnimation->start();

        update();
    }
}

void QBannerWidget::sltJump(int index)
{
    if (!x_isAnimation) {
        x_index = index;

        x_animationType = Jump;
        x_isAnimation = true;

        x_pLeftAnimation->setStartValue(QRect(115, 10, 503, 190));
        x_pLeftAnimation->setEndValue(QRect(0, 10, 503, 190));

        x_pCenterAnimation->setStartValue(QRect(this->rect().center().x(), this->rect().center().y(), 0, 0));
        x_pCenterAnimation->setEndValue(QRect(115, 0, 540, 200));

        x_pRightAnimation->setStartValue(QRect(115, 10, 503, 190));
        x_pRightAnimation->setEndValue(QRect(267, 10, 503, 190));

        x_pLeftAnimation->start();
        x_pRightAnimation->start();
        x_pCenterAnimation->start();

        update();
    }
}

void QBannerWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    if (x_imageList.count() < 3) {
        painter.drawText(this->geometry(), Qt::AlignCenter, QString::fromLocal8Bit("At least three images"));
    } else {
        drawLeft(&painter);

        drawRight(&painter);

        drawCenter(&painter);
    }
}

void QBannerWidget::drawLeft(QPainter *painter)
{

    painter->save();
    if (!x_isAnimation) {
        int _leftIndex = (x_index + 8 - 1) % 8;
        QImage _leftImage = QImage(x_imageList.at(_leftIndex)).scaled(503, 190);
        painter->drawImage(QRect(0, 10, 503, 190), getGrayImage(_leftImage));
    } else {
        if (x_animationType == Next) {
            QImage _leftImage = QImage(x_imageList.at(x_index)).scaled(503, 190);
            painter->drawImage(x_leftRect, getGrayImage(_leftImage));
        } else if (x_animationType == Prev) {
            int _leftIndex = (x_index + 8 - 2) % 8;
            QImage _leftImage = QImage(x_imageList.at(_leftIndex)).scaled(503, 190);
            painter->drawImage(x_leftRect, getGrayImage(_leftImage));
        } else if (x_animationType == Jump) {
            int _leftIndex = (x_index + 8 - 1) % 8;
            QImage _leftImage = QImage(x_imageList.at(_leftIndex)).scaled(503, 190);
            painter->drawImage(x_leftRect, getGrayImage(_leftImage));
        }
    }

    painter->restore();
}

void QBannerWidget::drawCenter(QPainter *painter)
{
    painter->save();
    if (!x_isAnimation) {
        painter->drawImage(QRect(115, 0, 540, 200), QImage(x_imageList.at(x_index)));
    } else {
        if (x_animationType == Next) {
            int _centerIndex = (x_index + 8 + 1) % 8;
            QImage _centerImage = QImage(x_imageList.at(_centerIndex));
            painter->drawImage(x_centerRect, _centerImage);
        } else if (x_animationType == Prev) {
            int _centerIndex = (x_index + 8 - 1) % 8;
            QImage _centerImage = QImage(x_imageList.at(_centerIndex));
            painter->drawImage(x_centerRect, _centerImage);
        } else if (x_animationType == Jump) {
            painter->drawImage(x_centerRect, QImage(x_imageList.at(x_index)));
        }
    }

    painter->restore();
}

void QBannerWidget::drawRight(QPainter *painter)
{
    painter->save();
    if (!x_isAnimation) {
        int _rightIndex = (x_index + 8 + 1) % 8;
        QImage _rightImage = QImage(x_imageList.at(_rightIndex)).scaled(503, 190);
        painter->drawImage(QRect(267, 10, 503, 190), getGrayImage(_rightImage));
    } else {
        if (x_animationType == Next) {
            int _rightIndex = (x_index + 8 + 2) % 8;
            QImage _rightImage = QImage(x_imageList.at(_rightIndex)).scaled(503, 190);
            painter->drawImage(x_rightRect, getGrayImage(_rightImage));
        } else if (x_animationType == Prev) {
            QImage _rightImage = QImage(x_imageList.at(x_index)).scaled(503, 190);
            painter->drawImage(x_rightRect, getGrayImage(_rightImage));
        } else if (x_animationType == Jump) {
            int _rightIndex = (x_index + 8 + 1) % 8;
            QImage _rightImage = QImage(x_imageList.at(_rightIndex)).scaled(503, 190);
            painter->drawImage(x_rightRect, getGrayImage(_rightImage));
        }
    }

    painter->restore();
}

QImage QBannerWidget::getGrayImage(const QImage &image)
{
    int w, h;
    w = image.width();
    h = image.height();

    QImage iGray(w, h, QImage::Format_RGB32);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QRgb pixel = image.pixel(i, j);
            int gray = qGray(pixel);
            QRgb grayPixel = qRgb(gray, gray, gray);
            iGray.setPixel(i, j, grayPixel);
        }
    }
    return iGray;
}

void QBannerWidget::slot_ValueChanged(const QVariant &value)
{
    QPropertyAnimation *_obj = (QPropertyAnimation *)(sender());

    if (_obj == x_pLeftAnimation) {
        x_leftRect = value.toRect();
    } else if (_obj == x_pCenterAnimation) {
        x_centerRect = value.toRect();
    } else if (_obj == x_pRightAnimation) {
        x_rightRect = value.toRect();
    }

    update();
}

void QBannerWidget::slot_finished()
{
    if (x_animationType == Next) {
        x_index = (x_index + 8 + 1) % 8;
    } else if (x_animationType == Prev) {
        x_index = (x_index + 8 - 1) % 8;
    }

    x_isAnimation = false;
}
