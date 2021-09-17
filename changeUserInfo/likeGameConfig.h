#pragma once

#include <QObject>
#include <QMap>
#include <QHash>

struct GameInfo
{
	QString gameId; //��Ϸid
	QString gameName; //��Ϸ��
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

	//���ݴ洢
	bool loadLikeGameListTable();
	void addLikeGameToMap(const QString& gameName);

	//��������
	bool readTableFile(const QString & strFilePath, QHash<QString, int>& keys, QJsonArray& jsonValues);

	//���ݻ�ȡ
	//��ȡ��Ϸ�б�
	QStringList getGameListByFirstWord(const QString& firstWord);
	//��ȡ����ĸ�б�
	QStringList getFirstWord();
	//��ȡ����ĸ
	QString getFirstWordByName(const QString& name);
	//��ȡ������Ϸ�б�
	QStringList getAllGameList();

signals:
	void sigUpdateFinished();

private:
	//��Ϸ���ݴ洢
	QMap<QString, GameInfo> m_mapLikeGame;
	//�洢����ĸ������Ϸ
	QMap<QString, QStringList> m_mapLikeGameByFirstWord;
	//����ĸ�洢
	//QStringList m_firstWordList;
	QStringList m_gameNameList;
};

#define vLikeGameMgr VLikeGameMgr::getInstance()
