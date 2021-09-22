#include "vloginsqlite.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include "vfileoperation.h"

vLoginSqlite::vLoginSqlite(QWidget *parent /* = Q_NULLPTR */)
{
	//���ݿ��ʼ��
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
	//�����ݿ�
	if (!m_loginDataBase.open()) {
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//���ݲ�ѯ
	QSqlQuery query;
	QString str = QString("select *from accountList");
	query.prepare(str);
	if (!query.exec()) {
		qInfo() << "Error: Fail to select from." << query.lastError();
	}
	else {
		//���ݱ���
		while (query.next()){
			m_userIdList = query.value(1).toStringList();
		}
	}
	//�����ر����ݿ�
	m_loginDataBase.close();
}

void vLoginSqlite::saveDataBase(loginUserInfo info)
{
	//�����ݿ�
	if (!m_loginDataBase.open()) {
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//�ж�keyֵ�Ƿ����
	QSqlQuery query;
	QString str = QString("select *from account where id = '%1'").arg(info.userId);
	query.prepare(str);
	if (!query.next()) {
		//�����ھͲ�������
		insertDataBase(info);
	}
	else {
		//���ھ���������� ��������
		updateDataBase(info);
	}
	//�����ر����ݿ�
	m_loginDataBase.close();
}

void vLoginSqlite::saveUserIdList(QStringList idList)
{
	//�����ݿ�
	if (!m_loginDataBase.open()) {
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//�ж�keyֵ�Ƿ����
	QSqlQuery query;
	QString str = QString("select *from accountList where id = '%1'").arg("List");
	query.prepare(str);
	if (!query.next()) {
		//�����ھͲ�������
		QString str = QString("insert into accountList values (?, ?)");
		query.prepare(str);
		query.addBindValue("List");
		query.addBindValue(idList);
		if (!query.exec()) {
			qInfo() << "Error: Fail to insert into." << query.lastError();
		}
	}
	else {
		//���ھ���������� ��������
		QString str = QString("update accountList set idList = :idList, where id = :id");
		query.prepare(str);
		query.bindValue(":id", "List");
		query.bindValue(":idList", idList);
		if (!query.exec()) {
			qInfo() << "Error: Fail to update" << query.lastError();
		}
	}
	//�����ر����ݿ�
	m_loginDataBase.close();
}

void vLoginSqlite::selectAllDataBase()
{
	//�����ݿ�
	if (!m_loginDataBase.open()) {
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//���ݲ�ѯ
	QSqlQuery query;
	QString str = QString("select *from account");
	query.prepare(str);
	if (!query.exec()) {
		qInfo() << "Error: Fail to select from." << query.lastError();
	}
	else {
		//���ݱ���
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
	//�����ر����ݿ�
	m_loginDataBase.close();
}

void vLoginSqlite::insertDataBase(loginUserInfo info)
{
	//���ݲ���
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
	//���ݸ���
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
	//�����ݿ�
	if (!m_loginDataBase.open()) {
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//ɾ������
	QSqlQuery query;
	QString str = QString("delete from account where id = ?");
	query.prepare(str);
	query.addBindValue(key);
	if (!query.exec()) {
		qInfo() << "Error: Fail to delete from." << query.lastError();
	}
	//�����ر����ݿ�
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
	//�����ݿ�
	if (!m_loginDataBase.open()){
		qInfo() << "Error: Failed to connect database." << m_loginDataBase.lastError();
		return;
	}
	//��������
	QSqlQuery query;
	QString str = QString("create table account (id string primary key, name string, password string, ispwd bool, isauto bool, processid string)");
	query.prepare(str);
	if (!query.exec()){
		qInfo() << "Error: Fail to create account table." << query.lastError();
	}
	//�����˻��б�
	QString strlist = QString("create table accountList (id string primary key, idList QStringList)");
	query.prepare(strlist);
	if (!query.exec()) {
		qInfo() << "Error: Fail to create accountList table." << query.lastError();
	}

	//�����ر����ݿ�
	m_loginDataBase.close();
}
