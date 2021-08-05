#include "chatMessageItem.h"
#include "imageTool.h"

ChatMessageItem::ChatMessageItem(QWidget *parent)
	:QWidget(parent), m_userId(QString())
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

	connect(ui.pushButton_min, &QPushButton::clicked, this, &ChatMessageItem::sigMinimized);
	connect(ui.pushButton_max, &QPushButton::clicked, this, &ChatMessageItem::sigMaxOrNomal);
	connect(ui.pushButton_Tclose, &QPushButton::clicked, this, &ChatMessageItem::sigClose);
	connect(ui.pushButton_close, &QPushButton::clicked, this, [this]() {
		emit sigDeleteItem(m_userId);
	});

}

ChatMessageItem::~ChatMessageItem()
{

}

void ChatMessageItem::setUserDataInfo(const userDataInfo& info)
{
	//��¼id
	m_userId = info.userId;
	//����ͷ��
	QPixmap logo(info.logoPath);
	if (!logo.isNull()) {
		logo = getRoundRectPixmap(logo, QSize(ui.label_logo->size()), 40);
		ui.label_logo->setScaledContents(true);
		ui.label_logo->setPixmap(logo);
	}
	//�����ǳ�
	ui.label_nickName->setText(info.nickName);
	//����ǩ��
	ui.label_text->setText(info.text);
}
