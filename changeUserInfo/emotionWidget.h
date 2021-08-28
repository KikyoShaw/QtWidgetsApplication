#pragma once

#include <QListWidget>
#include "ui_emotionWidget.h"

class QButtonGroup;

class EmotionWidget : public QWidget
{
	Q_OBJECT
public:
	explicit EmotionWidget(QWidget *parent = Q_NULLPTR);
	~EmotionWidget();

private:
	void initButtonGroup();

private slots:
	void sltButtonToggled(QAbstractButton * button, bool toggled);

private:
	Ui::emotion ui;
	//情感选项按钮管理器
	QButtonGroup *m_buttonGroup = nullptr;
};
