#include "likeGameConfig.h"
#include "pinyin.h"
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

VLikeGameMgr::VLikeGameMgr()
{
}

VLikeGameMgr::~VLikeGameMgr()
{
}

VLikeGameMgr & VLikeGameMgr::getInstance()
{
	static VLikeGameMgr m_instance;
	return m_instance;
}

void VLikeGameMgr::clear()
{
	m_mapLikeGame.clear();
	m_mapLikeGameByFirstWord.clear();
	m_gameNameList.clear();
}

bool VLikeGameMgr::loadLikeGameListTable()
{
	QHash<QString, int> hashKeys;
	QJsonArray jsonArray;
	QString path = "qrc/1000-23A98AF33723EB82D5AC9FBD44A4F7EA";
	if (!readTableFile(path, hashKeys, jsonArray)) {
		return false;
	}

	clear();

	for (int i = 0; i < jsonArray.size(); i++)
	{
		QJsonValue val = jsonArray.at(i);
		if (val.isNull()) continue;
		QJsonArray arr = val.toArray();
		GameInfo info;
		if (hashKeys.contains("gameId")) {
			int iGameId = hashKeys["gameId"];
			info.gameId = arr.at(iGameId).toString();
		}
		if (info.gameId.isEmpty()) continue;
		if (hashKeys.contains("gameName")) {
			int iGameName = hashKeys["gameName"];
			info.gameName = arr.at(iGameName).toString();
		}
		//  游戏数据存储
		m_mapLikeGame.insert(info.gameId, info);

		//加入首字母分类游戏数据结构
		if (!info.gameName.isEmpty()) {
			addLikeGameToMap(info.gameName);
		}
	}

	//修改发送信号
	emit sigUpdateFinished();

	//存储首字母
	//m_firstWordList = m_mapLikeGameByFirstWord.keys();
	//排序
	//qSort(m_firstWordList.begin(), m_firstWordList.end(), [=](auto p2, auto p1) {
	//	QLocale loc(QLocale::Chinese, QLocale::China);
	//	QCollator qcol(loc);
	//	int nResult = 0;
	//	nResult = qcol.compare(p1, p2);
	//	return nResult > 0 ? true : false;
	//});

	QList<QStringList> stringList = m_mapLikeGameByFirstWord.values();
	for (auto & each : stringList){
		m_gameNameList.append(each);
	}

	return false;
}

void VLikeGameMgr::addLikeGameToMap(const QString & gameName)
{
	//根据游戏名获取首字母
	QString firstWord = firstPinYinLetter(gameName);
	QStringList gameList = QStringList();
	if (m_mapLikeGameByFirstWord.contains(firstWord)) {
		gameList = m_mapLikeGameByFirstWord.value(firstWord);
	}
	gameList.append(gameName);
	m_mapLikeGameByFirstWord.insert(firstWord, gameList);
}

bool VLikeGameMgr::readTableFile(const QString & strFilePath, QHash<QString, int>& keys, QJsonArray & jsonValues)
{
	QFile file(strFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	QByteArray bArr = file.readAll();
	file.close();

	auto jsonDoc = QJsonDocument::fromJson(bArr);
	if (jsonDoc.isNull()) {
		return false;
	}

	//parse keys
	QJsonObject jsonObj = jsonDoc.object();
	QJsonValue jsonKeys = jsonObj.value("keys");
	if (jsonKeys.isNull()) {
		return false;
	}
	QJsonArray jsonArray = jsonKeys.toArray();
	int size = jsonArray.size();
	for (int i = 0; i < size; ++i) {
		keys.insert(jsonArray.at(i).toString(), i);
	}

	//parse values
	QJsonValue jsonValue = jsonObj.value("values");
	if (jsonValue.isNull()) {
		return false;
	}
	jsonValues = jsonValue.toArray();

	return true;
}

QStringList VLikeGameMgr::getGameListByFirstWord(const QString & firstWord)
{
	QStringList gameList = QStringList();
	if (m_mapLikeGameByFirstWord.contains(firstWord)) {
		gameList = m_mapLikeGameByFirstWord.value(firstWord);
	}
	return gameList;
}

QStringList VLikeGameMgr::getFirstWord()
{
	//return m_firstWordList;
	return m_mapLikeGameByFirstWord.keys();
}

QString VLikeGameMgr::getFirstWordByName(const QString & name)
{
	QString firstWord = QString();
	firstWord = firstPinYinLetter(name);
	return firstWord;
}

QStringList VLikeGameMgr::getAllGameList()
{
	return m_gameNameList;
}
