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
	//模糊
	QGraphicsBlurEffect *m_blurEffect = Q_NULLPTR;
	//透明度
	QGraphicsOpacityEffect *m_opacityEffect = Q_NULLPTR;
	//颜色着色
	QGraphicsColorizeEffect *m_colorizeEffect = Q_NULLPTR;
	//边框阴影
	QGraphicsDropShadowEffect *m_dropShadowEffect = Q_NULLPTR;

};
