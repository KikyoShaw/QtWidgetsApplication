#pragma once

#include <QObject>
#include <QMap>
#include <QHash>

struct GameInfo
{
	QString gameId; //游戏id
	QString gameName; //游戏名
};

class VLikeGameMgr : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(VLikeGameMgr)
	VLikeGameMgr();

public:
	~VLikeGameMgr();
	static VLikeGameMgr& getInstance();
	void clear();

	//数据存储
	bool loadLikeGameListTable();
	void addLikeGameToMap(const QString& gameName);

	//解析数据
	bool readTableFile(const QString & strFilePath, QHash<QString, int>& keys, QJsonArray& jsonValues);

	//数据获取
	//获取游戏列表
	QStringList getGameListByFirstWord(const QString& firstWord);
	//获取首字母列表
	QStringList getFirstWord();
	//获取首字母
	QString getFirstWordByName(const QString& name);
	//获取所有游戏列表
	QStringList getAllGameList();

signals:
	void sigUpdateFinished();

private:
	//游戏数据存储
	QMap<QString, GameInfo> m_mapLikeGame;
	//存储首字母分类游戏
	QMap<QString, QStringList> m_mapLikeGameByFirstWord;
	//首字母存储
	//QStringList m_firstWordList;
	QStringList m_gameNameList;
};

#define vLikeGameMgr VLikeGameMgr::getInstance()
