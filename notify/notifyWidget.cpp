#include "notifyWidget.h"
#include "notifymanager.h"
#include "imageTool.h"

NotifyWidget::NotifyWidget(NotifyManager* manager, QWidget *parent)
    : ArrangedWidget(manager, parent)
{
    ui.setupUi(this);
    m_manager = manager;
    connect(this, &NotifyWidget::visibleChanged, [this](bool visible) {
        if (visible)
        {
            int displayTime = m_data.value("displayTime", m_manager->displayTime()).toInt();
            QTimer::singleShot(displayTime, this, [this]() {
                showArranged(0);
                });
        }
        else deleteLater();
        });

    connect(ui.pushButton_close, &QPushButton::clicked, this, &QObject::deleteLater);
}

NotifyWidget::~NotifyWidget()
{}

QVariantMap NotifyWidget::data() const
{
    return m_data;
}

void NotifyWidget::setData(const QVariantMap& data)
{
    m_data = data;

    QPixmap icon;
    QString url = m_data.value("url").toString();
    QPixmap logo(url);
    if (!logo.isNull()) {
        logo = getRoundRectPixmap(logo, QSize(ui.label_logo->size()), 48);
        ui.label_logo->setScaledContents(true);
        ui.label_logo->setPixmap(logo);
    }

   /* QString title = data.value("title").toString();
    titleLabel->setText(title);*/

    // 计算可显示行数及长度
    QString body = m_data.value("body").toString();
    ui.label_text->setText(body);
}


