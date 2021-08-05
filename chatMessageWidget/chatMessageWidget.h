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
	//�����Ҳര��
	void creatChatWidget(const userDataInfo&);
	//���������Ŀ
	void creatChatItem(const userDataInfo&);
	//���������Ŀ����
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
	//�����ƶ�����ֵ
	QPoint m_point;
	bool m_bMove = false;
	//�Ҳര�ڼ�¼
	QMap<QString, ChatMessageItem*> m_mapChatWidget;
	//�����Ŀ��¼
	QMap<QString, QListWidgetItem*> m_mapChatItem;
};
