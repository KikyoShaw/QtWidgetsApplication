#include "NotifyCountWidget.h"
#include "notifymanager.h"
#include <QBoxLayout>
#include <QGraphicsDropShadowEffect>


NotifyCountWidget::NotifyCountWidget(NotifyManager* manager, QWidget *parent)
	: ArrangedWidget(manager, parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, true);

    iconLabel = new QLabel(this);
    iconLabel->setFixedSize(40, 40);
    iconLabel->setAlignment(Qt::AlignCenter);

    countLabel = new QLabel(this);
    countLabel->setObjectName("notify-count");
    countLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    countLabel->setFixedHeight(40);

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(iconLabel);
    mainLayout->addWidget(countLabel);
    mainLayout->setAlignment(Qt::AlignBottom);

    // ÎÄ×ÖÒõÓ°
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(2, 2);
    shadow->setBlurRadius(5);
    setGraphicsEffect(shadow);

    setStyleSheet("#notify-count {"
        "font: 20px Verdana;"
        "color: #dd424d;"
        "}");

    QPixmap icon = QPixmap(":/notify/image/message.png");
    icon = icon.scaled(QSize(32, 32), Qt::KeepAspectRatio);
    iconLabel->setPixmap(icon);

    flickerAnim = new QPropertyAnimation(this, "windowOpacity", this);
    flickerAnim->setStartValue(1);
    flickerAnim->setKeyValueAt(0.25, 0.1);
    flickerAnim->setKeyValueAt(0.5, 1);
    flickerAnim->setEndValue(1);
    flickerAnim->setDuration(2000);
    flickerAnim->setLoopCount(-1);

    connect(this, &NotifyCountWidget::visibleChanged, [this](bool visible) {
        if (visible) flickerAnim->start();
        else flickerAnim->stop();
        });
}

NotifyCountWidget::~NotifyCountWidget()
{}

void NotifyCountWidget::setCount(int count)
{
    countLabel->setNum(count);
}
