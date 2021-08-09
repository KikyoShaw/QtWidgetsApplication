#include "chatMessageBrowser.h"
#include <QScrollBar>
#include <QDebug>
#include <QPointer>
#include <QFile>

ChatMessageBrowser::ChatMessageBrowser(QWidget *parent)
	: QTextBrowser(parent)
{
	//避免滚动条出现
	verticalScrollBar()->setEnabled(false);
	horizontalScrollBar()->setEnabled(false);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	//禁止选择
	setEnabled(false);
	setReadOnly(true);
	//避免html格式改变文字方向
	auto textOption = document()->defaultTextOption();
	textOption.setAlignment(Qt::AlignLeft);
	textOption.setTextDirection(Qt::LeftToRight);
	document()->setDefaultTextOption(textOption);
}

ChatMessageBrowser::~ChatMessageBrowser()
{
}

void ChatMessageBrowser::setMaxWidth(int max)
{
	m_maxWidth = max;
	adjustSizeWithBorder();
}

void ChatMessageBrowser::adjustSizeWithBorder()
{
	QTextDocument doc;
	doc.setHtml(this->toHtml());
	//边距
	auto margins = this->contentsMargins();
	//设置宽
	int width = doc.size().width();
	width += margins.left() + margins.right();
	if (m_maxWidth > 0 && width > m_maxWidth) {
		width = m_maxWidth;
	}
	this->setFixedWidth(width);

	//设置高
	width -= margins.left() + margins.right();
	doc.setTextWidth(width);
	int height = doc.size().height();
	height += this->contentsMargins().top() + this->contentsMargins().bottom();
	this->setFixedHeight(height);
}

void ChatMessageBrowser::setBubbleImage(const QString & path)
{
	this->setStyleSheet(QString(
		"color: #FFFFFF;"
		"background-color: transparent;"
		"font: 12px Microsoft YaHei;"
		"border: none;"
		"border-image: url(%1) stretch;"
		"border-width: 20 100 20 130;"
		"padding: -15 -95 -15 -120;").arg(path));
}
