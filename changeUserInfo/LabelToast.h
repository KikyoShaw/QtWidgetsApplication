#pragma once

#include <QLabel>
#include <QTimer>
#include "ui_LabelToast.h"

class LabelToast : public QLabel
{
	Q_OBJECT

public:
	LabelToast(QWidget *parent = Q_NULLPTR);
	~LabelToast();

	void setToast(const QString &text);

protected:
	virtual void closeEvent(QCloseEvent *event);

private:
	QTimer m_timer;
};


//������������Ӧ��С������ÿ���ֶ�������С
class LabelToastAuto : public QWidget
{
	Q_OBJECT

public:
	LabelToastAuto(QWidget *parent = Q_NULLPTR);
	~LabelToastAuto();

	void setToast(const QString &text);
	void setBackgroundStyle(const QString &style);
	void setSpace(int space);

protected:
	virtual void closeEvent(QCloseEvent *event);

private:
	Ui::toastAuto ui;
	//��ʱ��
	QTimer m_timer;
	//�ܼ��
	int m_space = 0;
};
