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
    //ѡ���û�
    void sltShowAccountInfo(int index, const QString& accountUserId);
    //�Ƴ���ǰ��¼�б���ĳһ��;
    void sltRemoveAccount(int index, const QString& accountName);

    //void sltComboBoxHide();

private:
    //�������б�����(����QCOmboBox)
    QListWidget* m_AccountList = nullptr;

private:
    Ui::ComboBoxTestClass ui;
};
