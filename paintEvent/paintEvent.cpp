#include "paintEvent.h"
#include <QPainter>
#include <QVariantAnimation>
#include <QPropertyAnimation>
#include <QStyle>

static QVector<QColor> Color_Value = {
	Qt::yellow,
	Qt::green,
	Qt::blue,
	Qt::red,
};

paintEvents::paintEvents(QWidget *parent)
    : QWidget(parent), m_nAlpha(255)
	, m_nAtIndex(0), m_nAtIndex1(0)
{
    ui.setupUi(this);

	m_vAnimation = new QVariantAnimation(this);
	if (m_vAnimation) {
		m_vAnimation->setDuration(2000);
		m_vAnimation->setStartValue(0);
		m_vAnimation->setEndValue(5);
		m_vAnimation->setLoopCount(-1);
		m_vAnimation->start();
		connect(m_vAnimation, &QVariantAnimation::valueChanged, this, &paintEvents::sltAtIndexChanged);
	}

	m_vAnimation2 = new QVariantAnimation(this);
	if (m_vAnimation2) {
		m_vAnimation2->setDuration(1000);
		m_vAnimation2->setStartValue(0);
		m_vAnimation2->setEndValue(2);
		m_vAnimation2->setLoopCount(-1);
		m_vAnimation2->start();
		connect(m_vAnimation2, &QVariantAnimation::valueChanged, this, &paintEvents::sltAtIndex1Changed);
	}

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
		connect(m_pAnimation, &QVariantAnimation::valueChanged, this, &paintEvents::sltValueChanged);
	}
}

void paintEvents::sltAtIndexChanged(QVariant value)
{
	m_nAtIndex = value.toDouble();
	update();
}

void paintEvents::sltAtIndex1Changed(QVariant value)
{
	m_nAtIndex1 = value.toDouble();
	update();
}

QString paintEvents::getTextByWidth(const QFontMetrics & fm, const QString & text, int width)
{
	QString descText = QString();
	if (width <= 0)
		return descText;

	int offset = 0;
	do{
		descText = text.mid(0, text.length() - offset++);
	} while (!(fm.width(descText, width) <= width));

	return descText;
}

QStringList paintEvents::getTextLinesByRectSize(const QFontMetrics & fm, const QString & text, const QSize & size)
{
	QStringList splitLines = text.split('\n');
	QStringList result;
	if (size.width() <= 120){
		int ab = 0;
		ab++;
	}
	for (int i = 0; i < splitLines.count(); i++)
	{
		QString splitLine = splitLines[i];
		do{
			QString splitLineElidedText = getTextByWidth(fm, splitLine, size.width());
			if (!splitLineElidedText.isEmpty())
				result.append(splitLineElidedText);
			splitLine = splitLine.mid(splitLineElidedText.length(), splitLine.length() - splitLineElidedText.length());
		} while (!splitLine.isEmpty());
	}
	return result;
}

void paintEvents::sltValueChanged(QVariant value)
{
	m_nAlpha = value.toInt();
	this->update();
}

void paintEvents::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

	//绘制心
	//paintHeart(&painter);

	//绘制特殊文本
	//paintColorText(&painter);

	//绘制文字光晕特效
	//paintColorText2(&painter);

	//艺术字体
	paintColorText3(&painter);

	//
	//paintColorText4(&painter);

	//流光文字
	//paintAnimationText(&painter);

	//文字闪烁
	//paintAnimationText2(&painter);

	//文字颜色渐变动画
	//paintAnimationText3(&painter);
}

void paintEvents::paintHeart(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	double k = 10;
	QColor my_color(237, 162, 255, 255);
	QBrush my_brush(my_color);
	painter->setPen(Qt::red);
	painter->translate(this->width() / 2, this->height() / 2);
	QPainterPath polygonPath;
	polygonPath.setFillRule(Qt::WindingFill);
	float x = 16 * k * sin(0.0)*sin(0.0)*sin(0.0);
	float y = 13 * k * cos(0.0) - 5 * k*cos(0.0) - 2 * k*cos(0.0) - cos(0.0);
	polygonPath.moveTo(x, -y);
	for (double t = 0.01; t < 100; t += 0.05) {
		x = 16 * k * sin(k*t)*sin(k*t)*sin(k*t);
		y = 13 * k * cos(k*t) - 5 * k * cos(2 * k * t) - 2 * k * cos(3 * k * t) - cos(4 * k * t);
		polygonPath.lineTo(x, -y);
		//painter->drawPoint(x,-y);
		//painter->drawLine(0,0,x,-y);
		//painter->drawLine(0,0,x+100,-y);
	}
	painter->drawPath(polygonPath);
}

void paintEvents::paintColorText(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	QFont font = painter->font();
	font.setPixelSize(200);

	/*
	QLinearGradient：显示从起点到终点的渐变
	QRadialGradient：以圆心为中心显示渐变
	QConicalGradient：围绕一个中心点显示渐变
	*/

	QRadialGradient g(width() / 2, height() / 2, 100);
	g.setSpread(QGradient::ReflectSpread);
	//double s = 6;
	//g.setColorAt(0 / s, Qt::yellow);
	//g.setColorAt(1 / s, Qt::green);
	//g.setColorAt(2 / s, Qt::blue);
	//g.setColorAt(3 / s, Qt::red);
	//g.setColorAt(4 / s, Qt::magenta);
	//g.setColorAt(5 / s, Qt::cyan);
	//g.setColorAt(6 / s, Qt::white);

	// 设置显示模式
	/*
	QGradient::PadSpread
	QGradient::RepeatSpread
	QGradient::ReflectSpread
	*/
	//g.setSpread(QGradient::PadSpread);

	if (0 == m_nAtIndex) {
		g.setColorAt(1, Qt::yellow);
		g.setColorAt(0, Qt::yellow);
	}
	else if (1 == m_nAtIndex) {
		g.setColorAt(1, Qt::green);
		g.setColorAt(0, Qt::green);
	}
	else if (2 == m_nAtIndex) {
		g.setColorAt(1, Qt::blue);
		g.setColorAt(0, Qt::blue);
	}
	else if (3 == m_nAtIndex) {
		g.setColorAt(1, Qt::red);
		g.setColorAt(0, Qt::red);
	}

	QString str = QStringLiteral("十三先生");
	QFontMetrics fm(font);
	QRect rect = fm.boundingRect(this->rect(), Qt::AlignCenter, str);

	QPainterPath path;
	path.addText(rect.x(), rect.y() + fm.ascent(), font, str);

	painter->setOpacity(0.5);
	painter->fillPath(path, Qt::black);
	painter->strokePath(path, QPen(Qt::black, 6));

	painter->save();
	painter->setOpacity(1);
	painter->translate(-4, -3);
	painter->fillPath(path, g);
	painter->strokePath(path, QPen(Qt::darkMagenta, 4));
	painter->restore();

	painter->setOpacity(0.1);
	painter->fillRect(rect, g);
}

void paintEvents::paintColorText2(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	auto text = QStringLiteral("十三先生");
	QColor pColor(255, 20, 147);
	QFont font(QStringLiteral("微软雅黑"));
	font.setPixelSize(30);
	font.setWeight(QFont::Black);
	QFontMetrics fm(font);
	int textflags = Qt::AlignLeft | Qt::TextExpandTabs;
	QSize textsize = fm.size(textflags, text);
	//边缘留白
	int margin = 8;
	textsize.setWidth(textsize.width() + 2 * margin);
	textsize.setHeight(textsize.height() + 2 * margin);
	//文字处理
	QPainterPath pp(QPointF(margin, margin));
	qreal px = margin, py = margin + fm.ascent();
	foreach(const QString& line, text) {
		pp.addText(px, py, font, line);
		py += fm.lineSpacing();
	}
	//填充文字区域，做出光晕效果
	QPainterPathStroker pps;
	pps.setCapStyle(Qt::RoundCap);
	//pps.setJoinStyle(Qt::MiterJoin);
	pps.setWidth(3);
	QPainterPath path = pps.createStroke(pp).united(pp).simplified();
	QColor glow_color = QColor(255 - pColor.red(), 255 - pColor.green(), 255 - pColor.blue()).lighter(168);
	//绘制光晕
	QPixmap textpixmap(textsize);
	textpixmap.fill(QColor(0, 0, 0, 0));
	painter->begin(&textpixmap);
	painter->setFont(font);
	painter->fillPath(path, glow_color);
	painter->setPen(glow_color.lighter(88));
	painter->drawPath(path);
	painter->setPen(pColor);
	painter->drawText(QRect(margin, margin, textpixmap.width(), textpixmap.height()), textflags, text);
	painter->end();
}

void paintEvents::paintColorText3(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	auto text = QStringLiteral("忘川之畔，与君长相憩，烂泥之中，与君发相缠");
	auto rect = this->rect();
	int strokeWidth = 8;
	QColor strokeColor(Qt::red);
	auto option = QTextOption(Qt::AlignCenter);
	QFont font(QStringLiteral("微软雅黑"));
	font.setPixelSize(30);
	font.setBold(true);
	painter->save();
	painter->setFont(font);
	QStringList textList = getTextLinesByRectSize(painter->fontMetrics(), text, rect.size());
	int fontHeight = painter->fontMetrics().height();
	int lineHeight = painter->fontMetrics().lineSpacing();
	QPen oldPen = painter->pen();
	QPen strokePen = QPen(strokeColor, strokeWidth);
	for (int i = 0; i < textList.count(); i++){
		if (1 == m_nAtIndex1) {
			strokePen = QPen(QColor(255, 20, 147), strokeWidth);
			oldPen = QPen(Qt::green);
		}
		QString textLine = textList[i];
		QRect textLineBoundingRect = painter->fontMetrics().boundingRect(rect, option.flags(), textLine);
		QSize textLineSize = textLineBoundingRect.size();
		int offsetX = 0;
		int offsetY = i * lineHeight;
		//offset x
		if (option.alignment().testFlag(Qt::AlignLeft)){
			offsetX = 0;
		}
		if (option.alignment().testFlag(Qt::AlignRight)){
			offsetX = rect.width() - textLineBoundingRect.width();
		}
		if (option.alignment().testFlag(Qt::AlignHCenter)){
			offsetX = (rect.width() - textLineBoundingRect.width()) / 2;
		}
		//offset y
		if (option.alignment().testFlag(Qt::AlignTop)){
			//offsetY = 0;
		}
		if (option.alignment().testFlag(Qt::AlignBottom)){

			offsetY += rect.height() - textList.count() * lineHeight;
		}
		if (option.alignment().testFlag(Qt::AlignVCenter)){
			offsetY += (rect.height() - textList.count() * lineHeight) / 2;
		}
		textLineBoundingRect.setLeft(rect.left() + offsetX);
		textLineBoundingRect.setRight(textLineBoundingRect.left() + textLineSize.width());
		textLineBoundingRect.setTop(rect.top() + offsetY);
		textLineBoundingRect.setBottom(textLineBoundingRect.top() + textLineSize.height());
		QPainterPath strokePath;
		strokePath.addText(textLineBoundingRect.bottomLeft(), font, textLine);
		painter->strokePath(strokePath, strokePen);
		//painter->drawPath(strokePath);//debug
		painter->fillPath(strokePath, QBrush(oldPen.color()));
	}

	//for .debug
	//painter->drawText(rect, text, option);
	painter->restore();
}

void paintEvents::paintColorText4(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	auto text = QStringLiteral("忘川之畔，与君长相憩");
	auto myOpt = QTextOption(Qt::AlignCenter);
	QFont font(QStringLiteral("微软雅黑"));
	font.setPixelSize(30);
	painter->setFont(font);
	QRect sRect = painter->fontMetrics().boundingRect(QRect(150,150,width(),height()), myOpt.flags(), text);
	// 将渐变填充的范围设置成文字所在的范围
	QLinearGradient l(sRect.x(), sRect.y(), sRect.x() + sRect.width(),
		sRect.y() + sRect.height());

	// 设置彩虹色渐变效果，彩虹由赤橙黄绿青蓝紫的颜色组成
	// 因此我们除去起始点为红色，每隔1/6就设置一种颜色
	l.setColorAt(0, Qt::red);
	l.setColorAt(1.0 / 6, QColor(255, 97, 0));
	l.setColorAt(2.0 / 6, QColor(255, 255, 0));
	l.setColorAt(3.0 / 6, Qt::green);
	l.setColorAt(4.0 / 6, Qt::cyan);
	l.setColorAt(5.0 / 6, Qt::blue);
	l.setColorAt(1, QColor(255, 0, 255));

	// 这里并不使用painter，只需要QStyle即可实现效果
	// QPalette::Text为文本显示效果的role
	auto role = QPalette::Text;
	QPalette palette = this->palette();
	palette.setBrush(role, QBrush(l));
	// 注意最后一个参数role，只有设置了它才能让QStyle正确地绘制自定义的文本显示效果
	QApplication::style()->drawItemText(painter, sRect, Qt::AlignCenter,
		palette, true, text, role);
}

void paintEvents::paintAnimationText(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	auto text = QStringLiteral("十三先生");
	QFont font(QStringLiteral("华文彩云"));
	font.setPixelSize(30);
	painter->setFont(font);
	int x = 150;
	for (int i = 0; i < text.size(); i++) {
		auto color = Color_Value.value(i);
		painter->setPen(QPen(color));
		QString each = text[i];
		painter->drawText(QRect(x, 150, width(), height()), each);
		x += 30;
	}
}

void paintEvents::paintAnimationText2(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	auto text = QStringLiteral("十三先生");
	QFont font(QStringLiteral("华文彩云"));
	font.setPixelSize(30);
	painter->setFont(font);
	painter->setPen(QPen(QColor(128, 0, 128, m_nAlpha)));
	painter->drawText(QRect(150, 150, width(), height()),text);
}

void paintEvents::paintAnimationText3(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	auto text = QStringLiteral("十三先生");
	QFont font(QStringLiteral("微软雅黑"));
	font.setPixelSize(30);
	painter->setFont(font);
	int x = 150;
	for (int i = 0; i < text.size(); i++) {
		if (i < m_nAtIndex) {
			painter->setPen(QPen(Qt::green));
		}
		else {
			painter->setPen(QPen(Qt::white));
		}
		QString each = text[i];
		painter->drawText(QRect(x, 150, width(), height()), each);
		x += 30;
	}
}

void paintEvents::paintAnimationText4(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

}
