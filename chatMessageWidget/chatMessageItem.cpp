#include "chatMessageItem.h"
#include "imageTool.h"
#include "chatMessageBrowser.h"
#include <QDateTime>

ChatMessageItem::ChatMessageItem(QWidget *parent)
	:QWidget(parent), m_timeStamp(0)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint/* | Qt::WindowStaysOnTopHint*/);

	initUI();

	connect(ui.pushButton_min, &QPushButton::clicked, this, &ChatMessageItem::sigMinimized);
	connect(ui.pushButton_max, &QPushButton::clicked, this, &ChatMessageItem::sigMaxOrNomal);
	connect(ui.pushButton_Tclose, &QPushButton::clicked, this, &ChatMessageItem::sigClose);
	connect(ui.pushButton_close, &QPushButton::clicked, this, [this]() {
		clearUI();
		emit sigDeleteItem(m_userInfo.userId);
	});

	connect(ui.pushButton_send, &QPushButton::clicked, this, &ChatMessageItem::sltSendMessage);

}

ChatMessageItem::~ChatMessageItem()
{

}

void ChatMessageItem::setUserDataInfo(const userDataInfo& info)
{
	//记录信息
	m_userInfo = info;
	//设置头像
	QPixmap logo(info.logoPath);
	if (!logo.isNull()) {
		logo = getRoundRectPixmap(logo, QSize(ui.label_logo->size()), 40);
		ui.label_logo->setScaledContents(true);
		ui.label_logo->setPixmap(logo);
	}
	//设置昵称
	ui.label_nickName->setText(info.nickName);
	//设置签名
	ui.label_text->setText(info.text);
}

void ChatMessageItem::initUI()
{
	m_layoutMessages = ui.verticalLayout_4;
	m_layoutMessages->setAlignment(Qt::AlignTop);
}

void ChatMessageItem::clearUI()
{
	m_objectCleaner.clear();
	ui.textEdit->clear();
	m_timeStamp = 0;
	m_userInfo.clear();
}

void ChatMessageItem::sendMessage(const QString & text, bool isMyself)
{
	addTimeLabel();

	//添加消息
	QWidget *widget = new QWidget();
	QHBoxLayout *layout = new QHBoxLayout(widget);
	layout->setMargin(0);

	//头像
	auto logoWidget = new QWidget();
	logoWidget->setFixedSize(48, 48);
	QLabel *imageLogo = new QLabel(logoWidget);
	imageLogo->setGeometry(4, 4, 40, 40);
	imageLogo->setScaledContents(true);

	QString path = ":/qrc/image/11.jpg";
	if (!isMyself) {
		path = m_userInfo.logoPath;
	}

	QPixmap logo(path);
	if (!logo.isNull()) {
		logo = getRoundRectPixmap(logo, QSize(imageLogo->size()), 20);
		imageLogo->setPixmap(logo);
	}

	if (isMyself) {
		layout->addStretch();
	}
	else {
		layout->addWidget(logoWidget, 0, Qt::AlignTop);
	}

	auto *msgTextLb = new ChatMessageBrowser();
	//记录用于自适应大小
	m_chatbrowsers.append(QPointer<ChatMessageBrowser>(msgTextLb));
	if (msgTextLb) {
		msgTextLb->insertPlainText(text);
		int radius = 12;
		if (isMyself) {
			msgTextLb->setStyleSheet(QString("color: black;"
				"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(1, 233, 217, 1), stop:1 rgba(0, 248, 143, 1));"
				"font: 14px Microsoft YaHei;"
				"border: none;"
				"border-top-left-radius: %1px;"
				"border-top-right-radius:4px;"
				"border-bottom-left-radius: %1px;"
				"border-bottom-right-radius: %1px;"
				"padding: 5").arg(radius)
			);
		}
		else {
			msgTextLb->setStyleSheet(QString("color: rgba(255, 255, 255, 50%);"
				"background:#333334;"
				"font: 14px Microsoft YaHei;"
				"border: none;"
				"border-top-left-radius:4px;"
				"border-top-right-radius: %1px;"
				"border-bottom-left-radius: %1px;"
				"border-bottom-right-radius: %1px;"
				"padding: 5").arg(radius)
			);
		}
		layout->addWidget(msgTextLb, 0, Qt::AlignTop);
		//修改大小
		auto width = ui.scrollAreaWidgetContents->width() - 200;
		msgTextLb->setMaxWidth(width);
	}
	if (isMyself) {
		layout->addWidget(logoWidget, 0, Qt::AlignTop);
	}
	else {
		layout->addStretch();
	}

	m_objectCleaner.add(widget);
	m_layoutMessages->addWidget(widget);
}

void ChatMessageItem::addTimeLabel()
{
	//获取时间
	auto timeStamp = QDateTime::currentSecsSinceEpoch();
	auto timeDiff = timeStamp - m_timeStamp;
	QLabel *lbTimeStamp = nullptr;
	if (qAbs(timeDiff) > 60) {
		m_timeStamp = timeStamp;
		QDateTime time;
		time.setTime_t(m_timeStamp);
		QDate today = QDate::currentDate();
		//今天的日期只显示时间
		auto text = time.toString(Qt::DefaultLocaleShortDate);
		if (time.date() == today) {
			text = time.toString("hh:mm:ss");
		}
		lbTimeStamp = new QLabel(text);
		lbTimeStamp->setStyleSheet(
			"font-size:10px;"
			"color:#8E8E8E;"
		);
		lbTimeStamp->setAlignment(Qt::AlignCenter);
		m_objectCleaner.add(lbTimeStamp);
	}
	if (lbTimeStamp) {
		m_layoutMessages->addWidget(lbTimeStamp);
	}
}

void ChatMessageItem::resizeControls()
{
	auto width = ui.scrollAreaWidgetContents->width() - 200;
	for (auto item : m_chatbrowsers) {
		if (!item.isNull()) {
			item->setMaxWidth(width);
		}
	}
}

void ChatMessageItem::sltSendMessage()
{
	auto text = ui.textEdit->toPlainText();
	if (text.isEmpty())
		return;

	ui.textEdit->clear();
	sendMessage(text, true);

	sendMessage(text, false);
}

void ChatMessageItem::showEvent(QShowEvent *event)
{
	resizeControls();
}
void ChatMessageItem::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	resizeControls();
}
