#pragma once

#include <QWidget>
#include "ui_combo.h"

class QPropertyAnimation;

class ComBo : public QWidget
{
	Q_OBJECT
public:
	ComBo(QWidget *parent = Q_NULLPTR);
	~ComBo();

	void startComBoAnimation();

private:
	Ui::comBo ui;
	//��������
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
	//������
	int m_current;
};
