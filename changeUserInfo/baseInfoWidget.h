#pragma once

#include <QWidget>
#include "ui_baseInfoWidget.h"

class QButtonGroup;
class EmotionWidget;
class BirthInfoWidget;

enum SEX
{
	E_SEX_MAN = 1, //��
	E_SEX_WOMAN = 2, //Ů
};

class BaseInfoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit BaseInfoWidget(QWidget *parent = Q_NULLPTR);
	~BaseInfoWidget();
	
	//��ԭ����UI
	void clearUI();

private:
	void initButtonGroup();
	//��ʼ��������
	void initWidgets();

private slots:
	void sltButtonToggled(QAbstractButton * button, bool toggled);
	void sltOpenEmotionWidget();
	void sltOpenBirthInfoWidget();
	void sltEmotioneFocusState(bool state);
	void sltBirthFocusState(bool state);
	//�������ѡ��
	void sltSetEmotion(int emotion);
	//���ó���������
	void sltSetBirthInfo(int year, int month, int day);

private:
	virtual	bool eventFilter(QObject *obj, QEvent *event);

private:
	Ui::baseInfo ui;
	//��Ů��ť������
	QButtonGroup *m_RButtonGroup = Q_NULLPTR;
	//���״̬������
	EmotionWidget *m_emotionWidget = Q_NULLPTR;
	//��������������
	BirthInfoWidget *m_birthInfoWidget = Q_NULLPTR;
};