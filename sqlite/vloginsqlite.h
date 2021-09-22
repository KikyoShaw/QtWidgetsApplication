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

	//��ѯ�˻��б�
	void selectAccountDataBase();

	//��������
	void saveDataBase(loginUserInfo info);

	//�����û�id�б�
	void saveUserIdList(QStringList idList);

	//��ѯ��������
	void selectAllDataBase();

	//ɾ��ĳһ������
	void removeDataBaseByKey(const QString& key);

	//��ȡ����ʵ��
	QMap<QString, loginUserInfo> getLoginUserInfoMap();
	loginUserInfo getLoginUserInfoById(const QString& key);
	QStringList getUserIdList();

private:
	//�������
	void creatLoginTable();
	//��������
	void insertDataBase(loginUserInfo info);
	//��������
	void updateDataBase(loginUserInfo info);

private:
	QSqlDatabase m_loginDataBase;
	//�û���Ϣmap
	QMap<QString, loginUserInfo> m_loginUserMap;
	//�˻��б�
	QStringList m_userIdList;
};
