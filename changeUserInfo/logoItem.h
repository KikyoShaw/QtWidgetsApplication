#pragma once

#include <QWidget>
#include <QMenu>
#include "ui_logoItem.h"

class vLogoItem : public QWidget
{
	Q_OBJECT
public:
	explicit vLogoItem(QWidget *parent = Q_NULLPTR);
	~vLogoItem();

	//��ʾͼƬ
	void setImagePixMap(QPixmap imgPixMap);
	//��ǰ�Ƿ���ͼƬ
	bool getImagePixMapState();

private:
	void initMenu();
	void contextMenuEvent(QContextMenuEvent *event);

private slots:
	void sltMenuTrigger(QAction *pAction);

signals:
	void sigAddLogo();
	void sigDeleteLogo(QPixmap imgPixMap, vLogoItem *obj);

private:
	Ui::logoitem ui;
	//ͼƬ����
	QPixmap m_ImgPixMap;
	//�Ҽ�����
	QMenu m_menu;
	//ɾ��
	QAction *m_actionDelete = Q_NULLPTR;
	//���
	QAction *m_actionPhoto = Q_NULLPTR;
};