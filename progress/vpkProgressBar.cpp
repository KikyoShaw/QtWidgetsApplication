#include "vpkProgressBar.h"
#include <QPainter>
#include "qmath.h"
#include <QDebug>
#include <QVariantAnimation>

vpkProgressBar::vpkProgressBar(QWidget *parent)
	: QProgressBar(parent), m_nAtIndex(1.0)
{
	m_vAnimation = new QVariantAnimation(this);
	if (m_vAnimation) {
		m_vAnimation->setDuration(10000);
		m_vAnimation->setKeyValueAt(0, this->minimum());
		m_vAnimation->setKeyValueAt(0.5, this->maximum());
		m_vAnimation->setKeyValueAt(1, this->minimum());
		m_vAnimation->setLoopCount(-1);
		m_vAnimation->start();
		connect(m_vAnimation, &QVariantAnimation::valueChanged, this, &vpkProgressBar::sltAtIndexChanged);
	}
}

vpkProgressBar::~vpkProgressBar()
{
}

void vpkProgressBar::sltAtIndexChanged(QVariant value)
{
	m_nAtIndex = value.toDouble();
	update();
}

void vpkProgressBar::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	// øπæ‚≥› + ∆Ωª¨±ﬂ‘µ¥¶¿Ì
	painter.setRenderHints(QPainter::Antialiasing, true);
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	painter.setPen(Qt::transparent);

	painter.setBrush(QBrush(QColor(Qt::blue)));
	painter.drawRect(0, 0, width(), height());

	double percent = static_cast<double>(value()) / static_cast<double>(maximum());
	//double percent = static_cast<double>(m_nAtIndex) / static_cast<double>(maximum());
	double x_move = (percent * width());
	painter.setBrush(QBrush(Qt::red));
	painter.drawRect(x_move, 0, width(), height());
}
