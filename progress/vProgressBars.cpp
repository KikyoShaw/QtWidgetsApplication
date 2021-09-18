#include "vProgressBars.h"
#include <QPainter>
#include "qmath.h"
#include <QDebug>
#include <QVariantAnimation>

vProgressBars::vProgressBars(QWidget *parent)
	: QProgressBar(parent), m_nAtIndex(1.0)
{
	m_vAnimation = new QVariantAnimation(this);
	if (m_vAnimation) {
		m_vAnimation->setDuration(2000);
		//m_vAnimation->setStartValue(0);
		//m_vAnimation->setEndValue(100);
		m_vAnimation->setKeyValueAt(0, 0.0);
		m_vAnimation->setKeyValueAt(0.5, 0.5);
		m_vAnimation->setKeyValueAt(1, 1.0);
		m_vAnimation->setLoopCount(-1);
		m_vAnimation->start();
		connect(m_vAnimation, &QVariantAnimation::valueChanged, this, &vProgressBars::sltAtIndexChanged);
	}
}

vProgressBars::~vProgressBars()
{
}

void vProgressBars::sltAtIndexChanged(QVariant value)
{
	m_nAtIndex = value.toDouble();
	//int index = static_cast<int>(this->value()) / static_cast<int>(this->maximum());
	//index = qMin(value.toInt(), index);
	//m_nAtIndex = static_cast<double>(index);
	update();
}

void vProgressBars::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	// 抗锯齿 + 平滑边缘处理
	painter.setRenderHints(QPainter::Antialiasing, true);
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	painter.setBrush(QBrush(QColor(255, 255, 255, 51)));
	painter.setPen(Qt::transparent);
	QRect rect = this->rect();
	int radius = rect.height() / 2;
	painter.drawRoundedRect(rect, radius, radius);

	QPainterPath draw_path;
	double percent = static_cast<double>(value()) / static_cast<double>(maximum());
	double x_move = (percent * width());
	double theta = acos(abs(x_move - radius) / radius);
	double y_move = (sin(theta) * radius);
	if (x_move <= radius) {
		draw_path.moveTo(x_move, radius - y_move);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), static_cast<int>(180 - theta / M_PI * 180), static_cast<int>(theta / M_PI * 180));
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 180, static_cast<int>(theta / M_PI * 180));
		draw_path.lineTo(static_cast<int>(x_move), static_cast<int>(radius - y_move));
		//painter.setPen(QColor(0, 0, 0));
		//painter.drawArc(QRect(0,0,radius*2,radius*2),static_cast<int>(180-theta/M_PI*180)*16,static_cast<int>(theta/M_PI*180)*16);
		//painter.drawArc(QRect(0,0,radius*2,radius*2),180*16,static_cast<int>(theta/M_PI*180)*16);
		//painter.drawLine(QPointF(x_move,radius+y_move),QPointF(x_move,radius-y_move));
	}
	else if (x_move < radius * 2) {
		draw_path.moveTo(radius, 0);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 90, 90);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 180, 90);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 270, static_cast<int>(90 - theta / M_PI * 180));
		draw_path.lineTo(static_cast<int>(x_move), static_cast<int>(radius - y_move));
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 90 - static_cast<int>(90 - theta / M_PI * 180), static_cast<int>(90 - theta / M_PI * 180));
		//painter.setPen(QColor(0, 0, 0));
		//painter.drawArc(QRect(0, 0, radius * 2, radius * 2), 90 * 16, 90 * 16);
		//painter.drawArc(QRect(0, 0, radius * 2, radius * 2), 180 * 16, 90 * 16);
		//painter.drawArc(QRect(0, 0, radius * 2, radius * 2), 270 * 16, static_cast<int>(90 - theta / M_PI * 180) * 16);
		//painter.drawLine(QPointF(x_move, radius + y_move), QPointF(x_move, radius - y_move));
		//painter.drawArc(QRect(0, 0, radius * 2, radius * 2), (90 - static_cast<int>(90 - theta / M_PI * 180)) * 16, static_cast<int>(90 - theta / M_PI * 180) * 16);
	}
	else {
		draw_path.moveTo(radius, 0);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 90, 90);
		draw_path.lineTo(0, height() - radius * 2);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 180, 90);
		draw_path.lineTo(percent*width() - radius * 2, height());
		draw_path.arcTo(QRect(static_cast<int>(percent*width()) - radius * 2, 0, radius * 2, radius * 2), 270, 90);
		draw_path.lineTo(percent*width(), radius * 2);
		draw_path.arcTo(QRect(static_cast<int>(percent*width() - radius * 2), 0, radius * 2, radius * 2), 0, 90);
		draw_path.lineTo(radius * 2, 0);
	}
	//painter.fillPath(draw_path, QColor(QColor(43, 74, 255)));
	//整个进度条颜色
	//QLinearGradient linearGradient(rect.topLeft(), rect.topRight());
	//进度条部分颜色
	QLinearGradient linearGradient(QPoint(0, 0), QPoint(x_move, 0));
	linearGradient.setColorAt(1.0, QColor(255, 66, 213));
	linearGradient.setColorAt(m_nAtIndex, QColor(43, 74, 255));
	linearGradient.setColorAt(0.0, QColor(43, 74, 255));
	painter.fillPath(draw_path, QBrush(linearGradient));
}
