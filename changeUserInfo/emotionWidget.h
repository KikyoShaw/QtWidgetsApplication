#pragma once

#include <QListWidget>
#include "ui_emotionWidget.h"

class QButtonGroup;

enum EmotionValue
{
	E_Secret = 1, //保密
	E_Single = 2, //单身
	E_InLove = 3, //热恋中
	E_Married = 4, //已婚
	E_SameSex = 5, //同性
};

class EmotionWidget : public QWidget
{
	Q_OBJECT
public:
	explicit EmotionWidget(QWidget *parent = Q_NULLPTR);
	~EmotionWidget();

private:
	void initButtonGroup();

signals:
	void sigSelectIndex(int emotion);

private slots:
	void sltButtonToggled(QAbstractButton * button, bool toggled);

private:
	Ui::emotion ui;
	//情感选项按钮管理器
	QButtonGroup *m_buttonGroup = nullptr;
};
