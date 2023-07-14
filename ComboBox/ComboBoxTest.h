#pragma once

#include <QtWidgets/QWidget>
#include "ui_ComboBoxTest.h"
#include <QListWidget>

class ComboBoxTest : public QWidget
{
    Q_OBJECT

public:
    ComboBoxTest(QWidget *parent = nullptr);
    ~ComboBoxTest();

private slots:
    //选中用户
    void sltShowAccountInfo(int index, const QString& accountUserId);
    //移除当前登录列表中某一项;
    void sltRemoveAccount(int index, const QString& accountName);

    //void sltComboBoxHide();

private:
    //下拉框列表重载(用于QCOmboBox)
    QListWidget* m_AccountList = nullptr;

private:
    Ui::ComboBoxTestClass ui;
};
