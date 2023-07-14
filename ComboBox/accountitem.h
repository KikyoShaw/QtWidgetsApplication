#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QMouseEvent>

struct userInfo
{
	QString userId; //用户id
	QString nickName; //用户昵称
	QString path;    //头像路径
};

class AccountItem : public QWidget
{
	Q_OBJECT

public:
	AccountItem(QWidget *parent = NULL);
	~AccountItem();

	// 设置登录用户信息;
	void setAccountInfo(int index, const userInfo& );
	// 获取登录用户名称;
	QString getAccountName();
	// 获取当前itemWidget的index;
	int getItemWidgetIndex();
	//取消删除按钮
	void hideDeleButton();

public slots:
	// 删除当前用户item;
	void onRemoveAccount();

private:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	QPixmap scaledPixmap(const QPixmap& src, int width, int height);
	QPixmap generatePixmap(const QPixmap& src, const QSize& size, const int& radius);

Q_SIGNALS:
	// 通知登录界面显示当前用户信息;
	void signalShowAccountInfo(int index, QString accountName);
	// 通知登录界面删除当前用户信息;
	void signalRemoveAccount(int index, QString accountName);

private:
	bool m_mousePress;
	//控件
	QLabel *m_accountImage;
	QLabel *m_accountNumber;
	QToolButton *m_deleteButton;
	int m_index;
	//记录用户id
	QString m_accountUserId;
};

#endif // ACCOUNTITEM_H