#include "arrangedWidget.h"
#include "notifymanager.h"

static QPropertyAnimation *propertyAnimationOnTarget(QObject *target, const QByteArray &propertyName, const QVariant &endValue, int duration)
{
    QPropertyAnimation *animation = new QPropertyAnimation(target, propertyName, target);
    animation->setStartValue(target->property(propertyName));
    animation->setEndValue(endValue);
    animation->setDuration(duration);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    return animation;
}

template<typename func>
static inline void propertyAnimationOnTarget(QObject *target, const QByteArray &propertyName, const QVariant &endValue, int duration, func onFinished)
{
    QPropertyAnimation *animation = propertyAnimationOnTarget(target, propertyName, endValue, duration);
    QObject::connect(animation, &QPropertyAnimation::finished, target, onFinished);
}

ArrangedWidget::ArrangedWidget(NotifyManager *manager, QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint)
{
    m_manager = manager;
    m_posIndex = 0;

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFixedSize(manager->notifyWndSize());

    connect(manager, &QObject::destroyed, this, &QObject::deleteLater);
}

void ArrangedWidget::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton: emit clicked(); break;
    case Qt::RightButton: emit rclicked(); break;
    default: break;
    }
}

void ArrangedWidget::showArranged(int posIndex)
{
    if (m_posIndex == posIndex) return;
    m_posIndex = posIndex;
    if (posIndex <= 0) // 隐藏
    {
        if (!isVisible()) return;
        propertyAnimationOnTarget(this, "windowOpacity", 0, m_manager->animateTime(), [this](){
            hide();
            emit visibleChanged(false);
        });
        return;
    }

    // 计算提醒框的位置
    QSize wndsize = m_manager->notifyWndSize();
    QSize offset = QSize(wndsize.width(), wndsize.height()*posIndex + m_manager->spacing()*(posIndex-1));
    int posY = offset.height();
    if (posIndex == 6)
        posY -= 40;
    QPoint pos = m_manager->cornerPos() - QPoint(offset.width(), posY);

    if (!isVisible()) // 显示
    {
        show();
        move(pos);
        setWindowOpacity(0);
        propertyAnimationOnTarget(this, "windowOpacity", 1, m_manager->animateTime(), [this](){
            emit visibleChanged(true);
        });
    }
    else // 移动位置
    {
        propertyAnimationOnTarget(this, "pos", pos, m_manager->animateTime());
    }
}


