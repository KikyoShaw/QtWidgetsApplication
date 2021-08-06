#pragma once

#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineSettings>
#include "WebConnect.h"

class WebLobby : public QWebEngineView
{
	Q_OBJECT

public:
	WebLobby(QWidget *parent = nullptr);
	~WebLobby();
	void setAutoResize(bool status);
	//发送消息到Web
	void sendToWebPage(const QString &text);

protected:
	void showEvent(QShowEvent*);

private:
	void parseWebMessage(const QString &message);

signals:
	void sigReceiveFromWebPage(const QString &text);

private:
	WebConnect m_WebObject;
	QWebChannel m_WebChannel;
	int m_Resize;
	bool m_bAutoResize;
};
