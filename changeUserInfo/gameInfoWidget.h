#pragma once

#include <QWidget>
#include "ui_gameInfoWidget.h"

class GameInfoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit GameInfoWidget(QWidget *parent = Q_NULLPTR);
	~GameInfoWidget();

private:
	//»ñÈ¡Ê××ÖÄ¸
	QString getChineseSpell(const QString &text);

private:
	Ui::gameInfo ui;
};
