#include "animationprocessbar.h"
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QPaintEvent>
#include "qmath.h"

AnimationProcessBar::AnimationProcessBar(QWidget *parent)
	: QProgressBar(parent), m_nAlpha(255)
{
	m_pAnimation = new QPropertyAnimation();
	if (m_pAnimation) {
		m_pAnimation->setTargetObject(this);
		m_pAnimation->setPropertyName("alpha");
		m_pAnimation->setDuration(1000);
		m_pAnimation->setKeyValueAt(0, 255);
		m_pAnimation->setKeyValueAt(0.5, 50);
		m_pAnimation->setKeyValueAt(1, 255);
		m_pAnimation->setLoopCount(-1);
		m_pAnimation->start();
		connect(m_pAnimation, &QVariantAnimation::valueChanged, this, &AnimationProcessBar::sltValueChanged);
	}
}

AnimationProcessBar::~AnimationProcessBar()
{
}

void AnimationProcessBar::sltValueChanged(const QVariant &value)
{
	m_nAlpha = value.toInt();
	this->update();
}

void AnimationProcessBar::paintEvent(QPaintEvent *event)
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
	QLinearGradient linearGradient(QPoint(0, 0), QPoint(x_move, 0));
	linearGradient.setColorAt(1, QColor(255, 66, 213, m_nAlpha));
	linearGradient.setColorAt(0.0, QColor(43, 74, 255, m_nAlpha));
	painter.fillPath(draw_path, QBrush(linearGradient));
}
