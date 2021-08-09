#include "chatMessageWidget.h"
#include <QListWidgetItem>
#include <QMouseEvent>
#include "logoItem.h"

chatMessageWidget::chatMessageWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint/* | Qt::WindowStaysOnTopHint*/);
	setAttribute(Qt::WA_TranslucentBackground);
	connect(ui.listWidget, &QListWidget::itemSelectionChanged, this, &chatMessageWidget::sltItemSelect);
}

chatMessageWidget::~chatMessageWidget()
{
}

void chatMessageWidget::openChatWidget(const userDataInfo &info)
{
	//创建窗口
	creatChatWidget(info);
	creatChatItem(info);
}

void chatMessageWidget::creatChatWidget(const userDataInfo &info)
{
	ChatMessageItem *chatItem = Q_NULLPTR;
	if (m_mapChatWidget.contains(info.userId)) {
		chatItem = m_mapChatWidget.value(info.userId);
	}
	else {
		chatItem = new ChatMessageItem();
		chatItem->setUserDataInfo(info);
		connect(chatItem, &ChatMessageItem::sigMinimized, this, &chatMessageWidget::showMinimized);
		connect(chatItem, &ChatMessageItem::sigMaxOrNomal, this, [this]() {
			if (isMaximized()) {
				showNormal();
			}
			else {
				showMaximized();
			}
		});
		connect(chatItem, &ChatMessageItem::sigClose, this, &chatMessageWidget::close);
		connect(chatItem, &ChatMessageItem::sigDeleteItem, this, &chatMessageWidget::sltDeleteChatItem);
		m_mapChatWidget[info.userId] = chatItem;
		ui.stackedWidget->addWidget(chatItem);
	}
}

void chatMessageWidget::creatChatItem(const userDataInfo &info)
{
	//创建左边条目
	QListWidgetItem *item = Q_NULLPTR;
	if (m_mapChatItem.contains(info.userId)) {
		item = m_mapChatItem.value(info.userId);
	}
	else {
		auto widget = new LogoItem();
		widget->setLogo(info.logoPath);
		item = new QListWidgetItem(ui.listWidget);
		item->setData(Qt::UserRole, info.userId);
		ui.listWidget->setItemWidget(item, widget);
		ui.listWidget->addItem(item);
		m_mapChatItem[info.userId] = item;
	}
	//切换选中
	if (item) {
		ui.listWidget->setCurrentItem(item);
	}
	resizeItems();
}

void chatMessageWidget::resizeItems()
{
	int count = ui.listWidget->count();
	if (count <= 0) {
		close();
		return;
	}
	if (count > 1) {
		ui.widget->show();
	}
	else {
		ui.widget->hide();
	}
	resizeEvent(Q_NULLPTR);
}

void chatMessageWidget::sltItemSelect()
{
	auto item = ui.listWidget->currentItem();
	if (item == Q_NULLPTR) {
		return;
	}
	auto userId = item->data(Qt::UserRole).toString();
	if (!m_mapChatWidget.contains(userId)) {
		return;
	}
	auto chat = m_mapChatWidget.value(userId);
	if (chat) {
		ui.stackedWidget->setCurrentWidget(chat);
	}
}

void chatMessageWidget::sltDeleteChatItem(const QString & userId)
{
	if (userId.isEmpty()) {
		return;
	}
	if (!m_mapChatItem.contains(userId)) {
		return;
	}
	auto item = m_mapChatItem.value(userId);
	m_mapChatItem.remove(userId);
	ui.listWidget->removeItemWidget(item);
	delete item;
	resizeItems();
}

void chatMessageWidget::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void chatMessageWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void chatMessageWidget::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}

void chatMessageWidget::closeEvent(QCloseEvent * event)
{
	QWidget::closeEvent(event);
	emit sigCloseSignal();
}
