#pragma once

#include <QtWidgets/QWidget>
#include "ui_changeUserInfo.h"

class LabelToastAuto;
class vLogoItem;

class changeUserInfo : public QWidget
{
    Q_OBJECT

public:
    changeUserInfo(QWidget *parent = Q_NULLPTR);
	~changeUserInfo();

private:
	//初始化toast提示窗口
	void initToast();
	//窗口图片管理
	void initAddLogoButton();
	//添加logo窗口
	void addLogoWidget(QPixmap imgPixMap);
	//toast提示
	void showToast(const QString& text);
	//logo窗口清理
	void clearLogoItem();

private slots:
	void sltReadLocalImage();
	void sltDeleteLogo(QPixmap nPixMap, vLogoItem *obj);

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::changeUserInfoClass ui;
	//窗口移动属性值
	QPoint m_point;
	bool m_bMove = false;
	//自适应提示tip
	LabelToastAuto *m_labelToastAuto = nullptr;
	//图片数据
	QVector<QPixmap> m_pixMapVec;
	//图片窗口管理
	QVector<vLogoItem*> m_listItemWidgets;
};
