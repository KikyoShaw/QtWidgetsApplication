#include "myprocessbar.h"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QPaintEvent>
#include "qmath.h"

MyProcessBar::MyProcessBar(QWidget *parent) 
	: QProgressBar(parent)
{
    m_timer = new QTimer(this);
	if (m_timer) {
		connect(m_timer, &QTimer::timeout, this, &MyProcessBar::sltTimerOut);
		m_timer->start(50);
	}
}

MyProcessBar::~MyProcessBar()
{
}

void MyProcessBar::sltTimerOut()
{
	int _value = this->value();
	if (_value != 0){
		if (m_currvalue >= _value)
			m_currvalue = 0;
		m_currvalue++;
		update();
	}
}

void MyProcessBar::paintEvent(QPaintEvent *event)
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
	painter.fillPath(draw_path, QColor("#67B5B7"));

	if (x_move > radius) {
		QRect rect = this->rect();
		auto maxValue = this->maximum();
        double _width = static_cast<double>(m_currvalue)/static_cast<double>(maxValue)*rect.width();
        QLinearGradient gradient(0,0,_width, rect.height());
		gradient.setColorAt(1, QColor(255, 66, 213, 100));
		gradient.setColorAt(0.0, QColor(43, 74, 255, 100));
        painter.setBrush(gradient);
        QRectF rect2 = QRectF(rect.topLeft(),QSize(static_cast<int>(_width),static_cast<int>(rect.height())));
        painter.drawRoundedRect(rect2, radius, radius);
    }

}
