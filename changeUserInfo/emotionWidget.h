#pragma once

#include <QListWidget>
#include "ui_emotionWidget.h"

class QButtonGroup;

enum EmotionValue
{
	E_Secret = 1, //����
	E_Single = 2, //����
	E_InLove = 3, //������
	E_Married = 4, //�ѻ�
	E_SameSex = 5, //ͬ��
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
	//���ѡ�ť������
	QButtonGroup *m_buttonGroup = nullptr;
};
