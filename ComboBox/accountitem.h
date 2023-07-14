#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QMouseEvent>

struct userInfo
{
	QString userId; //�û�id
	QString nickName; //�û��ǳ�
	QString path;    //ͷ��·��
};

class AccountItem : public QWidget
{
	Q_OBJECT

public:
	AccountItem(QWidget *parent = NULL);
	~AccountItem();

	// ���õ�¼�û���Ϣ;
	void setAccountInfo(int index, const userInfo& );
	// ��ȡ��¼�û�����;
	QString getAccountName();
	// ��ȡ��ǰitemWidget��index;
	int getItemWidgetIndex();
	//ȡ��ɾ����ť
	void hideDeleButton();

public slots:
	// ɾ����ǰ�û�item;
	void onRemoveAccount();

private:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	QPixmap scaledPixmap(const QPixmap& src, int width, int height);
	QPixmap generatePixmap(const QPixmap& src, const QSize& size, const int& radius);

Q_SIGNALS:
	// ֪ͨ��¼������ʾ��ǰ�û���Ϣ;
	void signalShowAccountInfo(int index, QString accountName);
	// ֪ͨ��¼����ɾ����ǰ�û���Ϣ;
	void signalRemoveAccount(int index, QString accountName);

private:
	bool m_mousePress;
	//�ؼ�
	QLabel *m_accountImage;
	QLabel *m_accountNumber;
	QToolButton *m_deleteButton;
	int m_index;
	//��¼�û�id
	QString m_accountUserId;
};

#endif // ACCOUNTITEM_H