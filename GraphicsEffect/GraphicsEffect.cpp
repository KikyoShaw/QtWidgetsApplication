#include "GraphicsEffect.h"
#include <QGraphicsBlurEffect>
#include <QPixmap>
#include <QGraphicsOpacityEffect>
#include <QGraphicsColorizeEffect>
#include <QGraphicsDropShadowEffect>

GraphicsEffect::GraphicsEffect(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	m_blurEffect = new QGraphicsBlurEffect(this);
	if (m_blurEffect) {
		//数值越大，越模糊
		m_blurEffect->setBlurRadius(0);
		ui.label_blur->setGraphicsEffect(m_blurEffect);
	}

	m_opacityEffect = new QGraphicsOpacityEffect(this);
	if (m_opacityEffect) {
		//数字越大，越不透明
		m_opacityEffect->setOpacity(1);
		ui.label_opacity->setGraphicsEffect(m_opacityEffect);
	}

	m_colorizeEffect = new QGraphicsColorizeEffect(this);
	if (m_colorizeEffect) {
		m_colorizeEffect->setColor(QColor(0, 0, 0, 100));
		//0.0表示无效果，1.0表示完全着色，默认1.0
		m_colorizeEffect->setStrength(0.0);
		ui.label_colorize->setGraphicsEffect(m_colorizeEffect);
	}

	m_dropShadowEffect = new QGraphicsDropShadowEffect(this);
	if (m_dropShadowEffect) {
		m_dropShadowEffect->setColor(QColor(0, 0, 0));
		m_dropShadowEffect->setOffset(0, 0);
		m_dropShadowEffect->setBlurRadius(0);
		ui.label_dropShadow->setGraphicsEffect(m_dropShadowEffect);
	}

	connect(ui.horizontalSlider_blur, &QSlider::valueChanged, this, [=](int value) {
		if (m_blurEffect) {
			m_blurEffect->setBlurRadius(value);
		}
	});
	connect(ui.horizontalSlider_opacity, &QSlider::valueChanged, this, [=](int value) {
		if (m_opacityEffect) {
			double _value = 1.0 - (value / 10.0);
			m_opacityEffect->setOpacity(_value);
		}
	});
	connect(ui.horizontalSlider_colorize, &QSlider::valueChanged, this, [=](int value) {
		if (m_colorizeEffect) {
			double _value = value / 10.0;
			m_colorizeEffect->setStrength(_value);
		}
	});
	connect(ui.horizontalSlider_dropShadow, &QSlider::valueChanged, this, [=](int value) {
		if (m_dropShadowEffect) {
			m_dropShadowEffect->setBlurRadius(value);
		}
	});
}

GraphicsEffect::~GraphicsEffect()
{
}
