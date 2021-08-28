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
	//��ʼ��toast��ʾ����
	void initToast();
	//����ͼƬ����
	void initAddLogoButton();
	//���logo����
	void addLogoWidget(QPixmap imgPixMap);
	//toast��ʾ
	void showToast(const QString& text);
	//logo��������
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
	//�����ƶ�����ֵ
	QPoint m_point;
	bool m_bMove = false;
	//����Ӧ��ʾtip
	LabelToastAuto *m_labelToastAuto = nullptr;
	//ͼƬ����
	QVector<QPixmap> m_pixMapVec;
	//ͼƬ���ڹ���
	QVector<vLogoItem*> m_listItemWidgets;
};
