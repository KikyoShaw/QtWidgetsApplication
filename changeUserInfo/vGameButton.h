#pragma once

#include <QCheckBox>
#include "ui_vGameButton.h"

class GameButton : public QCheckBox
{
	Q_OBJECT
public:
	GameButton(QWidget *parent = Q_NULLPTR);
	~GameButton();

	void setContent(const QString& text);

signals:
	void sigToggled(const QString& text, bool toggled, GameButton *obj);

private:
	virtual void mousePressEvent(QMouseEvent *event);

private:
	Ui::vGameItem ui;
	// Ù–‘÷µ
	QString m_gameName;
};