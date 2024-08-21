#pragma once

#include "arrangedWidget.h"
#include <QtWidgets/QWidget>
#include "ui_notifyWidget.h"
#include <QLabel>
#include <QPushButton>

class NotifyWidget : public ArrangedWidget
{
    Q_OBJECT

public:
    NotifyWidget(NotifyManager* manager, QWidget *parent = nullptr);
    ~NotifyWidget();

    QVariantMap data() const;
    void setData(const QVariantMap& data);

private:
    QVariantMap m_data;
    //QFrame* background;
    //QPushButton* closeBtn;

private:
    Ui::notifyClass ui;
    NotifyManager* m_manager;
    int m_posIndex;
};
