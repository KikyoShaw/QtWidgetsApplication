#pragma once

#include <QWidget>
#include "ui_baseInfoWidget.h"

class QButtonGroup;
class EmotionWidget;

enum SEX
{
	E_SEX_MAN = 1, //男
	E_SEX_WOMAN = 2, //女
};

class BaseInfoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit BaseInfoWidget(QWidget *parent = Q_NULLPTR);
	~BaseInfoWidget();
	
	//还原清理UI
	void clearUI();

private:
	void initButtonGroup();
	//初始化下拉框
	void initWidgets();

private slots:
	void sltButtonToggled(QAbstractButton * button, bool toggled);
	void sltOpenEmotionWidget();

private:
	virtual	bool eventFilter(QObject *obj, QEvent *event);

private:
	Ui::baseInfo ui;
	//男女按钮管理器
	QButtonGroup *m_RButtonGroup = nullptr;
	//情感状态下拉框
	EmotionWidget *m_emotionWidget = Q_NULLPTR;
};