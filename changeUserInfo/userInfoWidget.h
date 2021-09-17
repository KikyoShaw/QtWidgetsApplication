#pragma once

#include <QWidget>
#include "ui_userInfoWidget.h"

class GameInfoWidget;

class UserInfoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit UserInfoWidget(QWidget *parent = Q_NULLPTR);
	~UserInfoWidget();

	//��ԭ����UI
	void clearUI();

private:
	void initGameWidget();

private slots:
	void sltOpenGameInfoWidget();
	void sltSlognFocusState(bool state);
	void sltChangeGameLabel(bool state);
	void sltSetMaxLengthAndGetLength();
	void sltSetSelectGameName(const QString & name, bool toggled);

private:
	Ui::userInfo ui;
	//��Ϸ�������б�
	GameInfoWidget *m_gameInfoWidget = Q_NULLPTR;
	//��Ϸ�洢
	QStringList m_gameList;
};