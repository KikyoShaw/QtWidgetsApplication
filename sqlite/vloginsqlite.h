#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include "WADefine.h"

class vLoginSqlite : public QObject
{
	Q_OBJECT

public:
	explicit vLoginSqlite(QWidget *parent = Q_NULLPTR);
	~vLoginSqlite();

	//查询账户列表
	void selectAccountDataBase();

	//保存数据
	void saveDataBase(loginUserInfo info);

	//保存用户id列表
	void saveUserIdList(QStringList idList);

	//查询所有数据
	void selectAllDataBase();

	//删除某一条数据
	void removeDataBaseByKey(const QString& key);

	//获取数据实体
	QMap<QString, loginUserInfo> getLoginUserInfoMap();
	loginUserInfo getLoginUserInfoById(const QString& key);
	QStringList getUserIdList();

private:
	//创建表格
	void creatLoginTable();
	//插入数据
	void insertDataBase(loginUserInfo info);
	//更新数据
	void updateDataBase(loginUserInfo info);

private:
	QSqlDatabase m_loginDataBase;
	//用户信息map
	QMap<QString, loginUserInfo> m_loginUserMap;
	//账户列表
	QStringList m_userIdList;
};
