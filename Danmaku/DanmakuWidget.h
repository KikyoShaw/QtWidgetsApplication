#pragma once

#include <QWidget>
#include <QColor>
#include <QString>

#include <memory>

class QHideEvent;
class QPaintEvent;
class QResizeEvent;
class QShowEvent;

class DanmakuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DanmakuWidget(QWidget *parent = nullptr);
    ~DanmakuWidget() override;

    bool sendText(const QString &text, const QColor &color = Qt::white);

    int fontSize() const;
    int laneCount() const;
    int effectiveLaneCount() const;
    qreal opacity() const;
    qreal scrollSpeed() const;
    bool isPaused() const;
    int queuedCount() const;
    int visibleCount() const;

public slots:
    void setFontSize(int size);
    void setLaneCount(int count);
    void setOpacity(qreal value);
    void setScrollSpeed(qreal pixelsPerSecond);
    void setPaused(bool paused);
    void clear();

signals:
    void settingsChanged();
    void statisticsChanged();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    class Private;
    std::unique_ptr<Private> d;
};