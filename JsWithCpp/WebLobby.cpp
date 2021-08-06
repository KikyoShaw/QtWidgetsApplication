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
	//��ֹ�Ҽ��˵�
	setContextMenuPolicy(Qt::NoContextMenu);
	//����ҳ��͸��
	page()->setBackgroundColor(Qt::transparent);
	//���ػ�����
	//page()->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
	//page()->runJavaScript("CppToJs()");
	//ע������JS�˵���
	m_WebChannel.registerObject("MainWindow", &m_WebObject);
	page()->setWebChannel(&m_WebChannel);
	//ͨ�ù��ܽ���
	connect(&m_WebObject, &WebConnect::sigReceiveFromJs, this, &WebLobby::parseWebMessage, Qt::UniqueConnection);
	//����ź����ڲ�ͨ�õĹ��ܽ���
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
	//������QSGRendererInterface::Software��ż������Ῠס����Ҫ�ı��С��������
	//QT�ٷ�bug��https://bugreports.qt.io/browse/QTBUG-68566
	//if (m_bAutoResize) {
	//	setFixedHeight(height() + m_Resize);
	//	m_Resize *= -1;
	//}
	return __super::showEvent(event);
}

void WebLobby::parseWebMessage(const QString & message)
{
	qInfo().noquote() << QStringLiteral("Web��Ϣ:") << message;
	// ����json��ʽ
	QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8());
	if (jsonDoc.isNull()) {
		return;
	}
	if (jsonDoc.isObject()) {
		QJsonObject obj = jsonDoc.object();
		int action = obj.value("action").toInt(-1);
	}
}
