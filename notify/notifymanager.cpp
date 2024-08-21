#include "notifymanager.h"
#include "notifyWidget.h"
#include "NotifyCountWidget.h"
#include <QApplication>
#include <QScreen>

NotifyManager::NotifyManager(QObject *parent) : QObject(parent)
{
    m_maxCount = 5;
    m_displayTime = 10000;
    m_animateTime = 300;
    m_spacing = 20;
    m_notifyWndSize = QSize(300, 110);
    m_isShowQueueCount = true;

    setCornerMargins(10, 30);
    m_notifyCount = new NotifyCountWidget(this);
}

void NotifyManager::notify(const QString &title, const QString &body, const QString &url, const QVariantMap &data)
{
    QVariantMap tmp = data;
    tmp.insert("title", title);
    tmp.insert("body", body);
    tmp.insert("url", url);
    m_dataQueue.enqueue(tmp);
    showNext();
}

void NotifyManager::setMaxCount(int count)
{
    m_maxCount = count;
}

int NotifyManager::displayTime() const
{
    return m_displayTime;
}

void NotifyManager::setDisplayTime(int displayTime)
{
    m_displayTime = displayTime;
}

int NotifyManager::animateTime() const
{
    return m_animateTime;
}

void NotifyManager::setAnimateTime(int animateTime)
{
    m_animateTime = animateTime;
}

int NotifyManager::spacing() const
{
    return m_spacing;
}

void NotifyManager::setSpacing(int spacing)
{
    m_spacing = spacing;
}

QPoint NotifyManager::cornerPos() const
{
    return m_cornerPos;
}

void NotifyManager::setCornerMargins(int right, int bottom)
{
    QRect desktopRect = QApplication::primaryScreen()->availableGeometry();
    QPoint bottomRignt = desktopRect.bottomRight();
    m_cornerPos = QPoint(bottomRignt.x() - right, bottomRignt.y() - bottom);
}

QSize NotifyManager::notifyWndSize() const
{
    return m_notifyWndSize;
}

void NotifyManager::setNotifyWndSize(int width, int height)
{
    m_notifyWndSize = QSize(width, height);
}

void NotifyManager::setShowQueueCount(bool isShowQueueCount)
{
    m_isShowQueueCount = isShowQueueCount;
    if (!m_isShowQueueCount) m_notifyCount->showArranged(0);
}

void NotifyManager::showNext()
{
    if (m_notifyList.size() >= m_maxCount || m_dataQueue.isEmpty())
    {
        showQueueCount();
        return;
    }

    NotifyWidget*notify = new NotifyWidget(this);
    m_notifyList.append(notify);
    notify->showArranged(m_notifyList.size());
    notify->setData(m_dataQueue.dequeue());
    showQueueCount();

    connect(notify, &QObject::destroyed, this, [notify, this](){
        int index = m_notifyList.indexOf(notify);
        m_notifyList.removeAt(index);
        for (; index<m_notifyList.size(); index++)
            m_notifyList[index]->showArranged(index+1);
        QTimer::singleShot(m_animateTime, this, [this](){
            showNext();
        });
    });

    connect(notify, &ArrangedWidget::clicked, this, [notify, this](){
        notify->deleteLater();
        QVariantMap data = notify->data();
        QTimer::singleShot(0, this, [this, data](){
            emit notifyDetail(data); // 保证窗口先销毁，避免模式窗口阻塞事件
        });
    });

    connect(notify, &ArrangedWidget::rclicked, this, [notify](){
        notify->deleteLater();
    });
}

void NotifyManager::showQueueCount()
{
    if (!m_isShowQueueCount) return;
    if (!m_dataQueue.isEmpty())
    {
        m_notifyCount->showArranged(m_maxCount+1);
        m_notifyCount->setCount(m_dataQueue.size());
    }
    else m_notifyCount->showArranged(0);
}
