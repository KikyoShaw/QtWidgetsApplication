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

	//��ʼ��ѡ��
	void initGameCheck(const QString& text);
	//��ʾ����
	void showWidget();

private:
	void clearData();
	void initGameList();
	//������ַ���ʾ�ؼ�
	void addFirstWordObject(const QString& text);
	//�����Ϸ��ʾ�ؼ�
	void addGameObject(const QString& text);
	//�����������������
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
	//���ڹ���
	QObjectCleanupHandler m_objectHandler;
	QObjectCleanupHandler m_searchObjectHandler;
	//���ݴ洢ʵ��
	QVector<GameButton*> m_gameButtonVec;
	//���ݻ���
	QStringList m_gameList;
};
