#pragma once

#include <QtWidgets/QWidget>
#include "ui_GraphicsEffect.h"

class QGraphicsBlurEffect;
class QGraphicsOpacityEffect;
class QGraphicsColorizeEffect;
class QGraphicsDropShadowEffect;

class GraphicsEffect : public QWidget
{
    Q_OBJECT

public:
    GraphicsEffect(QWidget *parent = Q_NULLPTR);
	~GraphicsEffect();

private:
    Ui::GraphicsEffectClass ui;
	//ģ��
	QGraphicsBlurEffect *m_blurEffect = Q_NULLPTR;
	//͸����
	QGraphicsOpacityEffect *m_opacityEffect = Q_NULLPTR;
	//��ɫ��ɫ
	QGraphicsColorizeEffect *m_colorizeEffect = Q_NULLPTR;
	//�߿���Ӱ
	QGraphicsDropShadowEffect *m_dropShadowEffect = Q_NULLPTR;

};
