#include "combo.h"
#include <QPropertyAnimation>
#include <QPainter>

static QVector<QString> Z_Nums = {
	":/number/qrc/number/z0.png",
	":/number/qrc/number/z1.png",
	":/number/qrc/number/z2.png",
	":/number/qrc/number/z3.png",
	":/number/qrc/number/z4.png",
	":/number/qrc/number/z5.png",
	":/number/qrc/number/z6.png",
	":/number/qrc/number/z7.png",
	":/number/qrc/number/z8.png",
	":/number/qrc/number/z9.png",
};

ComBo::ComBo(QWidget *parent)
	:QWidget(parent), m_current(0)
{
	ui.setupUi(this);

	m_pAnimation = new QPropertyAnimation(ui.label, "geometry");
	if (m_pAnimation) {
		m_pAnimation->setDuration(250);
		//connect(m_pAnimation, &QPropertyAnimation::finished, ui.label, &QLabel::hide);
	}
}

ComBo::~ComBo()
{
}

void ComBo::startComBoAnimation()
{
	m_current++;
	auto combo = QString::number(m_current);
	//计算宽
	int w = 44;
	for (auto s : combo) {
		if (s == '1') {
			w += 30;
		}
		else {
			w += 50;
		}
	}
	//绘制
	QPixmap temp(w, 100);
	temp.fill(Qt::transparent);
	int y = 10;
	w = 44;
	QPainter painter(&temp);
	QPixmap xImage = ":/number/qrc/number/zx.png";
	painter.drawPixmap(0, y - 2, xImage.scaled(44, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	for (auto s : combo) {
		if (s == '1') {
			auto num = s.digitValue();
			QPixmap numPixmap = Z_Nums.value(num);
			painter.drawPixmap(w, y, numPixmap.scaled(30, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			w += 30;
		}
		else {
			auto num = s.digitValue();
			QPixmap numPixmap = Z_Nums.value(num);
			painter.drawPixmap(w, y, numPixmap.scaled(50, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
			w += 50;
		}
	}
	painter.end();
	ui.label->resize(temp.size());
	ui.label->setPixmap(temp);
	ui.label->setScaledContents(true);

	//大小处理
	ui.label->move(25, 25);
	if (m_pAnimation) {
		auto w = ui.label->width() + ui.label->width() * 0.2;
		auto h = ui.label->height() + ui.label->height() * 0.2;
		auto x = 25 - (ui.label->width() * 0.2) / 2;
		auto y = 25 - (ui.label->height() * 0.2) / 2;
		m_pAnimation->setStartValue(QRect(x, y, w, h));
		m_pAnimation->setEndValue(ui.label->geometry());
		//开启动画
		m_pAnimation->start();
	}
	
}
