#pragma once

#include <QTextBrowser>

class ChatMessageBrowser : public QTextBrowser
{
	Q_OBJECT

public:
	ChatMessageBrowser(QWidget *parent = nullptr);
	~ChatMessageBrowser();

	//设置宽度，用于自适应大小
	void setMaxWidth(int max);

	//设置气泡
	void setBubbleImage(const QString& path);

private:
	//自适应大小
	void adjustSizeWithBorder();

private:
	int m_maxWidth = 0;
};
