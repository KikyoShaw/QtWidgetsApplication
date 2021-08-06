#include "WebLobby.h"
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>

WebLobby::WebLobby(QWidget *parent)
	: QWebEngineView(parent)
	, m_Resize(1)
	, m_bAutoResize(true)
{
	//setEnabled(false);
	//qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-logging");
	settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
	settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
	settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
	//禁止右键菜单
	setContextMenuPolicy(Qt::NoContextMenu);
	//设置页面透明
	page()->setBackgroundColor(Qt::transparent);
	//隐藏滑动条
	//page()->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
	//page()->runJavaScript("CppToJs()");
	//注册对象给JS端调用
	m_WebChannel.registerObject("MainWindow", &m_WebObject);
	page()->setWebChannel(&m_WebChannel);
	//通用功能解析
	connect(&m_WebObject, &WebConnect::sigReceiveFromJs, this, &WebLobby::parseWebMessage, Qt::UniqueConnection);
	//这个信号用于不通用的功能解析
	connect(&m_WebObject, &WebConnect::sigReceiveFromJs, this, &WebLobby::sigReceiveFromWebPage, Qt::UniqueConnection);
}

WebLobby::~WebLobby()
{
}

void WebLobby::setAutoResize(bool status)
{
	m_bAutoResize = status;
}

void WebLobby::sendToWebPage(const QString & text)
{
	qInfo().noquote() << "send to web" << endl << text;
	m_WebObject.sendToJs(text);
}

void WebLobby::showEvent(QShowEvent *event)
{
	//设置了QSGRendererInterface::Software后，偶尔界面会卡住，需要改变大小才能正常
	//QT官方bug：https://bugreports.qt.io/browse/QTBUG-68566
	//if (m_bAutoResize) {
	//	setFixedHeight(height() + m_Resize);
	//	m_Resize *= -1;
	//}
	return __super::showEvent(event);
}

void WebLobby::parseWebMessage(const QString & message)
{
	qInfo().noquote() << QStringLiteral("Web消息:") << message;
	// 解析json格式
	QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());
	if (jsonDoc.isNull()) {
		return;
	}
	if (jsonDoc.isObject()) {
		QJsonObject obj = jsonDoc.object();
		int action = obj.value("action").toInt(-1);
	}
}
