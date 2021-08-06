#include "JsWithCpp.h"
#include <QFileInfo>
#include <QDir>

JsWithCpp::JsWithCpp(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	loadWeb();

	//接收Web端的消息
	connect(ui.webEngineView, &WebLobby::sigReceiveFromWebPage, this, &JsWithCpp::parseWebMessage, Qt::UniqueConnection);

	//发送消息到web端
	connect(ui.pushButton, &QPushButton::clicked, this, &JsWithCpp::sendMessageToWeb, Qt::UniqueConnection);

	ui.textEdit->append("Hello Qt");

	ui.textEdit->setContextMenuPolicy(Qt::NoContextMenu);
	ui.lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
}

JsWithCpp::~JsWithCpp()
{
}

void JsWithCpp::loadWeb()
{
	QFileInfo jsFileInfo(QDir::currentPath() + "/qrc/qwebchannel.js");
	if (!jsFileInfo.exists()) {
		if (!QFile::copy(":/qwebchannel.js", jsFileInfo.absoluteFilePath()))
			return;
	}

	QFile file(":/index.html");
	if (!file.open(QFile::ReadOnly))
		return;

	ui.webEngineView->page()->setHtml(file.readAll());
}

void JsWithCpp::sendMessageToWeb()
{
	//ui.webEngineView->page()->runJavaScript("CppToJs()");
	auto text = ui.lineEdit->text();
	ui.textEdit->append("Qt: " + text);
	ui.webEngineView->sendToWebPage(text);
	ui.lineEdit->clear();
}

void JsWithCpp::parseWebMessage(const QString & message)
{
	ui.textEdit->append("JS: " + message);
}
