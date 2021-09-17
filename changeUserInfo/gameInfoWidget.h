#pragma once

#include <QWidget>
#include <QObjectCleanupHandler>
#include "ui_gameInfoWidget.h"

class GameButton;

class GameInfoWidget : public QWidget
{
	Q_OBJECT
public:
	explicit GameInfoWidget(QWidget *parent = Q_NULLPTR);
	~GameInfoWidget();

	//初始化选中
	void initGameCheck(const QString& text);
	//显示窗体
	void showWidget();

private:
	void clearData();
	void initGameList();
	//添加首字符显示控件
	void addFirstWordObject(const QString& text);
	//添加游戏显示控件
	void addGameObject(const QString& text);
	//搜索这边添加搜索结果
	void addFirstWordResult(const QString& text);
	void addGameNameResult(const QString& text);

private slots:
	void sltGameButtonToggled(const QString& name, bool toggled, GameButton *obj);
	void sltGameSearch(const QString& text);

signals:
	void sigChangeParentUI(bool state);
	void sigSelectGameName(const QString& name, bool toggled);

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	virtual void hideEvent(QHideEvent *event);

private:
	Ui::gameInfo ui;
	//窗口管理
	QObjectCleanupHandler m_objectHandler;
	QObjectCleanupHandler m_searchObjectHandler;
	//数据存储实体
	QVector<GameButton*> m_gameButtonVec;
	//数据缓存
	QStringList m_gameList;
};
