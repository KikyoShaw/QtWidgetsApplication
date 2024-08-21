#ifndef NOTIFYMANAGER_H
#define NOTIFYMANAGER_H

#include <QtCore>

class NotifyWidget;
class NotifyCountWidget;
class NotifyManager : public QObject
{
    Q_OBJECT

public:
    explicit NotifyManager( QObject *parent = 0);

    void notify(const QString &title, const QString &body, const QString& url, const QVariantMap &data = QVariantMap());

    void setMaxCount(int count);

    int displayTime() const;
    void setDisplayTime(int displayTime);

    int animateTime() const;
    void setAnimateTime(int animateTime);

    int spacing() const;
    void setSpacing(int spacing);

    QPoint cornerPos() const;
    void setCornerMargins(int right, int bottom);

    QSize notifyWndSize() const;
    void setNotifyWndSize(int width, int height);

    void setShowQueueCount(bool isShowQueueCount);

signals:
    void notifyDetail(const QVariantMap &data);

private:
    void showNext();
    void showQueueCount();

    QQueue<QVariantMap> m_dataQueue;
    QList<NotifyWidget *> m_notifyList;
    NotifyCountWidget* m_notifyCount;

    int m_maxCount;
    bool m_isShowQueueCount;
    int m_displayTime;
    int m_animateTime;
    int m_spacing;
    QPoint m_cornerPos;
    QSize m_notifyWndSize;
};

#endif // NOTIFYMANAGER_H
