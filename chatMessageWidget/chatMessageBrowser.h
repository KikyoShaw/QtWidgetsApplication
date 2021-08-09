#pragma once

#include <QTextBrowser>

class ChatMessageBrowser : public QTextBrowser
{
	Q_OBJECT

public:
	ChatMessageBrowser(QWidget *parent = nullptr);
	~ChatMessageBrowser();

	//���ÿ�ȣ���������Ӧ��С
	void setMaxWidth(int max);

	//��������
	void setBubbleImage(const QString& path);

private:
	//����Ӧ��С
	void adjustSizeWithBorder();

private:
	int m_maxWidth = 0;
};
