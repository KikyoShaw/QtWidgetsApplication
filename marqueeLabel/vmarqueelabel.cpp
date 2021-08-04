#include "vmarqueelabel.h"
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QStyleOption>

VMarqueeLabel::VMarqueeLabel(QWidget *parent)
	: QLabel(parent)
{
	connect(&m_Timer, &QTimer::timeout, this, [=] {
		if (isVisible()) {
			m_PosX -= 1;
			if (parent) {
				parent->update();
			}
			else {
				update();
			}
		}
	});
}

VMarqueeLabel::~VMarqueeLabel()
{
	clear();
}

void VMarqueeLabel::clear()
{
	m_Timer.stop();
	m_PosX = ORIGIN_X;
	QLabel::clear();
}

void VMarqueeLabel::setText(const QString& text)
{
	clear();
	QString content = QStringLiteral("<font style='font-family:MicrosoftYaHei;font-size:14px;color: #01EEC3;'>%1</font>").arg(text);
	m_Timer.start(10);
	QLabel::setText(content);
	adjustSize();
	m_PosX = width();
}

void VMarqueeLabel::paintEvent(QPaintEvent * evt)
{
	//计算x,y坐标
	QTextDocument doc;
	doc.setHtml(text());
	int left = doc.documentLayout()->documentSize().width();
	if (m_PosX <= -left) {
		//m_PosX = ORIGIN_X;
		m_PosX = width();
	}
	int height = doc.documentLayout()->documentSize().height();
	int y = (this->height() - height) / 2;
	//绘制
	QPainter painter(this);
	//先画背景
	QStyleOption opt;
	opt.init(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	painter.translate(m_PosX, y);
	doc.drawContents(&painter);
}
