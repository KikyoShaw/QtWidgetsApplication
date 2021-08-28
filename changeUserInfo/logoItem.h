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

	//显示图片
	void setImagePixMap(QPixmap imgPixMap);
	//当前是否有图片
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
	//图片数据
	QPixmap m_ImgPixMap;
	//右键功能
	QMenu m_menu;
	//删除
	QAction *m_actionDelete = Q_NULLPTR;
	//相册
	QAction *m_actionPhoto = Q_NULLPTR;
};