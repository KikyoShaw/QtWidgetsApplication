#pragma once

#include <QtWidgets/QWidget>
#include "chatMessageItem.h"
#include "ui_chatMessageWidget.h"

class QListWidgetItem;

class chatMessageWidget : public QWidget
{
    Q_OBJECT

public:
    chatMessageWidget(QWidget *parent = Q_NULLPTR);
	~chatMessageWidget();

	void openChatWidget(const userDataInfo&);

private:
	//创建右侧窗口
	void creatChatWidget(const userDataInfo&);
	//创建左侧条目
	void creatChatItem(const userDataInfo&);
	//设置左边条目隐藏
	void resizeItems();

protected slots:
	void sltItemSelect();
	void sltDeleteChatItem(const QString& userId);

signals:
	void sigCloseSignal();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void closeEvent(QCloseEvent *event);

private:
    Ui::chatWidget ui;
	//窗口移动属性值
	QPoint m_point;
	bool m_bMove = false;
	//右侧窗口记录
	QMap<QString, ChatMessageItem*> m_mapChatWidget;
	//左侧条目记录
	QMap<QString, QListWidgetItem*> m_mapChatItem;
};
