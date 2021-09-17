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
	//连击动画
	QPropertyAnimation *m_pAnimation = Q_NULLPTR;
	//连击数
	int m_current;
};
