#include "vloginsqlite.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "vfileoperation.h"

vLoginSqlite::vLoginSqlite(QWidget *parent /* = Q_NULLPTR */)
{
	//数据库初始化
	if (QSqlDatabase::contains("qt_sql_default_connection")){
		m_loginDataBase = QSqlDatabase::database("qt_sql_default_connection");
	}
	else
	{
		m_loginDataBase = QSqlDatabase::addDatabase("QSQLITE");
		m_loginDataBase.setDatabaseName("login.db");
		//m_loginDataBase.setUserName("WAAudio");
		//m_loginDataBase.setPassword("a123456");
	}

	creatLoginTable();
	selectAllDataBase();
}

vLoginSqlite::~vLoginSqlite()
{
}

void vLoginSqlite::selectAccountDataBase()
{
	//打开数据库
	if (!m_loginDataBase.open()) {
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//数据查询
	QSqlQuery query;
	QString str = QString("select *from accountList");
	query.prepare(str);
	if (!query.exec()) {
		qInfo() << "Error: Fail to select from." << query.lastError();
	}
	else {
		//数据遍历
		while (query.next()){
			m_userIdList = query.value(1).toStringList();
		}
	}
	//结束关闭数据库
	m_loginDataBase.close();
}

void vLoginSqlite::saveDataBase(loginUserInfo info)
{
	//打开数据库
	if (!m_loginDataBase.open()) {
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//判断key值是否存在
	QSqlQuery query;
	QString str = QString("select *from account where id = '%1'").arg(info.userId);
	query.prepare(str);
	if (!query.next()) {
		//不存在就插入数据
		insertDataBase(info);
	}
	else {
		//存在就在其基础上 更新数据
		updateDataBase(info);
	}
	//结束关闭数据库
	m_loginDataBase.close();
}

void vLoginSqlite::saveUserIdList(QStringList idList)
{
	//打开数据库
	if (!m_loginDataBase.open()) {
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//判断key值是否存在
	QSqlQuery query;
	QString str = QString("select *from accountList where id = '%1'").arg("List");
	query.prepare(str);
	if (!query.next()) {
		//不存在就插入数据
		QString str = QString("insert into accountList values (?, ?)");
		query.prepare(str);
		query.addBindValue("List");
		query.addBindValue(idList);
		if (!query.exec()) {
			qInfo() << "Error: Fail to insert into." << query.lastError();
		}
	}
	else {
		//存在就在其基础上 更新数据
		QString str = QString("update accountList set idList = :idList, where id = :id");
		query.prepare(str);
		query.bindValue(":id", "List");
		query.bindValue(":idList", idList);
		if (!query.exec()) {
			qInfo() << "Error: Fail to update" << query.lastError();
		}
	}
	//结束关闭数据库
	m_loginDataBase.close();
}

void vLoginSqlite::selectAllDataBase()
{
	//打开数据库
	if (!m_loginDataBase.open()) {
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//数据查询
	QSqlQuery query;
	QString str = QString("select *from account");
	query.prepare(str);
	if (!query.exec()) {
		qInfo() << "Error: Fail to select from." << query.lastError();
	}
	else {
		//数据遍历
		while (query.next())
		{
			loginUserInfo info;
			info.userId = query.value(0).toString();
			info.nickName = query.value(1).toString();
			info.passWord = query.value(2).toString();
			info.passWord = vvUtility::encodeData(info.passWord, true);
			info.isPwd = query.value(3).toBool();
			info.isAuto = query.value(4).toBool();
			if (!m_loginUserMap.contains(info.userId)) {
				m_loginUserMap.insert(info.userId, info);
			}
			info.processId = query.value(5).toString();
		}
	}
	//结束关闭数据库
	m_loginDataBase.close();
}

void vLoginSqlite::insertDataBase(loginUserInfo info)
{
	//数据插入
	QSqlQuery query;
	QString str = QString("insert into account values (?, ?, ?, ?, ?, ?)");
	query.prepare(str);
	query.addBindValue(info.userId);
	query.addBindValue(info.nickName);
	query.addBindValue(vvUtility::encodeData(info.passWord, false));
	query.addBindValue(info.isPwd);
	query.addBindValue(info.isAuto);
	query.addBindValue(info.processId);
	if (!query.exec()) {
		qInfo() << "Error: Fail to insert into." << query.lastError();
	}
}

void vLoginSqlite::updateDataBase(loginUserInfo info)
{
	//数据更新
	QSqlQuery query;
	QString str = QString("update account set name = :name,password = :password, ispwd = :ispwd, isauto = :isauto, processid = :processid  where id = :id");
	query.prepare(str);
	query.bindValue(":id", info.userId);
	query.bindValue(":name", info.nickName);
	query.bindValue(":password", vvUtility::encodeData(info.passWord, false));
	query.bindValue(":ispwd", info.isPwd);
	query.bindValue(":isauto", info.isAuto);
	//query.bindValue(":processid", 1);
	if (!query.exec()) {
		qInfo() << "Error: Fail to update" << query.lastError();
	}
}

void vLoginSqlite::removeDataBaseByKey(const QString & key)
{
	//打开数据库
	if (!m_loginDataBase.open()) {
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//删除数据
	QSqlQuery query;
	QString str = QString("delete from account where id = ?");
	query.prepare(str);
	query.addBindValue(key);
	if (!query.exec()) {
		qInfo() << "Error: Fail to delete from." << query.lastError();
	}
	//结束关闭数据库
	m_loginDataBase.close();
}

QMap<QString, loginUserInfo> vLoginSqlite::getLoginUserInfoMap()
{
	return m_loginUserMap;
}

loginUserInfo vLoginSqlite::getLoginUserInfoById(const QString & key)
{
	return m_loginUserMap.value(key);
}

QStringList vLoginSqlite::getUserIdList()
{
	return m_userIdList;
}

void vLoginSqlite::creatLoginTable()
{
	//打开数据库
	if (!m_loginDataBase.open()){
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//创建表名
	QSqlQuery query;
	QString str = QString("create table account (id string primary key, name string, password string, ispwd bool, isauto bool, processid string)");
	query.prepare(str);
	if (!query.exec()){
		qInfo() << "Error: Fail to create account table." << query.lastError();
	}
	//创建账户列表
	QString strlist = QString("create table accountList (id string primary key, idList QStringList)");
	query.prepare(strlist);
	if (!query.exec()) {
		qInfo() << "Error: Fail to create accountList table." << query.lastError();
	}

	//结束关闭数据库
	m_loginDataBase.close();
}
