#include "vProgressBar.h"
#include <QPainter>
#include "qmath.h"
#include <QDebug>

vProgressBar::vProgressBar(QWidget *parent)
	: QProgressBar(parent)
{

}

vProgressBar::~vProgressBar()
{

}

void vProgressBar::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	// 抗锯齿 + 平滑边缘处理
	painter.setRenderHints(QPainter::Antialiasing, true);
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	painter.setBrush(QBrush(QColor(255,255,255,51)));
	painter.setPen(Qt::transparent);
	QRect rect = this->rect();
	int radius = rect.height() / 2;
	painter.drawRoundedRect(rect, radius, radius);
	QPainterPath draw_path;
	double percent = static_cast<double>(value()) / static_cast<double>(maximum());
	double x_move = (percent * width());
	double theta = acos(abs(x_move - radius) / radius);
	double y_move = (sin(theta) * radius);
	if (x_move <= radius) { //左边
		draw_path.moveTo(x_move, radius - y_move);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), static_cast<int>(180 - theta / M_PI * 180), static_cast<int>(theta / M_PI * 180));
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 180, static_cast<int>(theta / M_PI * 180));
		draw_path.lineTo(static_cast<int>(x_move), static_cast<int>(radius - y_move));
	}
	else if (x_move > (width() - radius)) { //右边
		/*double x_right = x_move - (width() - radius);
		double y_right = sqrt(pow(20, 2) - pow(x_right, 2));
		qInfo() << y_right;*/
		/*draw_path.moveTo(width() - radius, 0);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 90, 90);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 180, 90);
		draw_path.lineTo(width() - radius, radius * 2);
		draw_path.lineTo(width() - radius, 0);*/

		draw_path.moveTo(radius, 0);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 90, 90);
		draw_path.lineTo(0, height() - radius * 2);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 180, 90);

		draw_path.lineTo(x_move - radius * 2, height());
		draw_path.arcTo(QRect(static_cast<int>(x_move - radius * 2), 0, radius * 2, radius * 2), 270, 90);
		draw_path.lineTo(x_move, radius * 2);
		draw_path.arcTo(QRect(static_cast<int>(x_move - radius * 2), 0, radius * 2, radius * 2), 0, 90);
		draw_path.lineTo(radius * 2, 0);
	}
	else { //中间部分
		draw_path.moveTo(x_move, 0);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 90, 90);
		draw_path.arcTo(QRect(0, 0, radius * 2, radius * 2), 180, 90);
		draw_path.lineTo(x_move, radius * 2);
		draw_path.lineTo(x_move, 0);
	}
	//整个进度条颜色
	//QLinearGradient linearGradient(rect.topLeft(), rect.topRight());
	//进度条部分颜色
	QLinearGradient linearGradient(QPoint(0,0), QPoint(x_move, 0));
	linearGradient.setColorAt(1, QColor(255, 66, 213));
	linearGradient.setColorAt(0.0, QColor(43, 74, 255));
	painter.fillPath(draw_path, QBrush(linearGradient));
}
