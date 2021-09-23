#include "suspensionScrollBar.h"
#include <QFile>
#include <QDebug>

SuspensionScrollBar::SuspensionScrollBar(Qt::Orientation type /* = Qt::Vertical */, QWidget *parent /* = Q_NULLPTR */)
	:QScrollBar(parent)
{
	//默认垂直滑动条
	this->setOrientation(type);
	//默认长度
	this->setRange(0, 0);
	//默认样式
	auto path = ":/qss/qrc/qss/whiteScrollbar.qss";
	QFile QSS(path);
	if (QSS.open(QFile::ReadOnly)) {
		QString style(QSS.readAll());
		this->setStyleSheet(style);
	}
	//初始状态隐藏滑动条
	this->setVisible(false);
}

SuspensionScrollBar::~SuspensionScrollBar()
{
}

void SuspensionScrollBar::setScrollBarStyleSheet(const QString & style)
{
	if (!style.isEmpty()) {
		this->setStyleSheet(style);
	}
}

void SuspensionScrollBar::setScrollBarStyleSheetByQSS(const QString & qssPath)
{
	QFile QSS(qssPath);
	if (QSS.open(QFile::ReadOnly)) {
		QString style(QSS.readAll());
		this->setStyleSheet(style);
	}
}

void SuspensionScrollBar::sltRangeChanged(int min, int max)
{
	//如果最大长度都不大于0，那么滑动条不出现
	if (0 >= max) {
		this->setVisible(false);
		return;
	}
	this->setMinimum(min);
	this->setRange(0, max);
	this->setPageStep(0.75 * (this->height() + max));
	this->setVisible(true);
}

void SuspensionScrollBar::sltValueChanged(int value)
{
	this->setValue(value);
}



ScrollBarListView::ScrollBarListView(QWidget *parent)
	:QListView(parent)
{
	//设置滑动条按像素滑动,避免空白显示
	this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	//关闭控件本身滑动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//初始化垂直滑动条类
	m_verticalScrollBar = new SuspensionScrollBar(Qt::Vertical, this);
	if (m_verticalScrollBar) {
		//重新绑定滑动条事件
		connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), m_verticalScrollBar, SLOT(sltValueChanged(int)));
		connect(m_verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sltHorizontalValueChanged(int)));
		connect(this->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), m_verticalScrollBar, SLOT(sltRangeChanged(int, int)));
	}
	//初始化水平滑动条类
	m_horizontalScrollBar = new SuspensionScrollBar(Qt::Horizontal, this);
	if (m_horizontalScrollBar) {
		//重新绑定滑动条事件
		connect(this->horizontalScrollBar(), SIGNAL(valueChanged(int)), m_horizontalScrollBar, SLOT(sltValueChanged(int)));
		connect(m_horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sltVerticalValueChanged(int)));
		connect(this->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), m_horizontalScrollBar, SLOT(sltRangeChanged(int, int)));
	}
}

ScrollBarListView::~ScrollBarListView()
{
}

void ScrollBarListView::setVerticalScrollBarVisible(bool visible)
{
	if (m_verticalScrollBar) {
		m_verticalScrollBar->setVisible(visible);
	}
}

void ScrollBarListView::setHorizontalScrollBarVisible(bool visible)
{
	if (m_horizontalScrollBar) {
		m_horizontalScrollBar->setVisible(visible);
	}
}

void ScrollBarListView::sltVerticalValueChanged(int value)
{
	this->verticalScrollBar()->setValue(value);
}

void ScrollBarListView::sltHorizontalValueChanged(int value)
{
	this->horizontalScrollBar()->setValue(value);
}

void ScrollBarListView::resizeEvent(QResizeEvent * event)
{
	if (m_verticalScrollBar) {
		auto posW = m_verticalScrollBar->width();
		auto posX = this->width() - posW;
		m_verticalScrollBar->setGeometry(posX, 1, posW, this->height() - 2);
	}
	if (m_horizontalScrollBar) {
		auto posH = m_verticalScrollBar->width();
		int posY = this->height() - posH;
		m_horizontalScrollBar->setGeometry(1, posY, this->height() - 2, posH);
	}
	QListView::resizeEvent(event);
}



ScrollBarListWidget::ScrollBarListWidget(QWidget *parent)
	:QListWidget(parent)
{
	//设置滑动条按像素滑动,避免空白显示
	this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	//关闭控件本身滑动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//初始化垂直滑动条类
	m_verticalScrollBar = new SuspensionScrollBar(Qt::Vertical, this);
	if (m_verticalScrollBar) {
		//重新绑定滑动条事件
		connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), m_verticalScrollBar, SLOT(sltValueChanged(int)));
		connect(m_verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sltHorizontalValueChanged(int)));
		connect(this->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), m_verticalScrollBar, SLOT(sltRangeChanged(int, int)));
	}
	//初始化水平滑动条类
	m_horizontalScrollBar = new SuspensionScrollBar(Qt::Horizontal, this);
	if (m_horizontalScrollBar) {
		//重新绑定滑动条事件
		connect(this->horizontalScrollBar(), SIGNAL(valueChanged(int)), m_horizontalScrollBar, SLOT(sltValueChanged(int)));
		connect(m_horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sltVerticalValueChanged(int)));
		connect(this->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), m_horizontalScrollBar, SLOT(sltRangeChanged(int, int)));
	}
}

ScrollBarListWidget::~ScrollBarListWidget()
{
}

void ScrollBarListWidget::setVerticalScrollBarVisible(bool visible)
{
	if (m_verticalScrollBar) {
		m_verticalScrollBar->setVisible(visible);
	}
}

void ScrollBarListWidget::setHorizontalScrollBarVisible(bool visible)
{
	if (m_horizontalScrollBar) {
		m_horizontalScrollBar->setVisible(visible);
	}
}

void ScrollBarListWidget::sltVerticalValueChanged(int value)
{
	this->verticalScrollBar()->setValue(value);
}

void ScrollBarListWidget::sltHorizontalValueChanged(int value)
{
	this->horizontalScrollBar()->setValue(value);
}

void ScrollBarListWidget::resizeEvent(QResizeEvent * event)
{
	if (m_verticalScrollBar) {
		auto posW = m_verticalScrollBar->width();
		auto posX = this->width() - posW;
		m_verticalScrollBar->setGeometry(posX, 1, posW, this->height() - 2);
	}
	if (m_horizontalScrollBar) {
		auto posH = m_verticalScrollBar->width();
		int posY = this->height() - posH;
		m_horizontalScrollBar->setGeometry(1, posY, this->height() - 2, posH);
	}
	QListWidget::resizeEvent(event);
}



ScrollBarScrollArea::ScrollBarScrollArea(QWidget *parent)
	:QScrollArea(parent)
{
	//关闭控件本身滑动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//初始化垂直滑动条类
	m_verticalScrollBar = new SuspensionScrollBar(Qt::Vertical, this);
	if (m_verticalScrollBar) {
		//重新绑定滑动条事件
		connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), m_verticalScrollBar, SLOT(sltValueChanged(int)));
		connect(m_verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sltVerticalValueChanged(int)));
		connect(this->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), m_verticalScrollBar, SLOT(sltRangeChanged(int, int)));
	}
	//初始化水平滑动条类
	m_horizontalScrollBar = new SuspensionScrollBar(Qt::Horizontal, this);
	if (m_horizontalScrollBar) {
		//重新绑定滑动条事件
		connect(this->horizontalScrollBar(), SIGNAL(valueChanged(int)), m_horizontalScrollBar, SLOT(sltValueChanged(int)));
		connect(m_horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sltHorizontalValueChanged(int)));
		connect(this->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), m_horizontalScrollBar, SLOT(sltRangeChanged(int, int)));
	}
}

ScrollBarScrollArea::~ScrollBarScrollArea()
{
}

void ScrollBarScrollArea::setVerticalScrollBarVisible(bool visible)
{
	if (m_verticalScrollBar) {
		m_verticalScrollBar->setVisible(visible);
	}
}

void ScrollBarScrollArea::setHorizontalScrollBarVisible(bool visible)
{
	if (m_horizontalScrollBar) {
		m_horizontalScrollBar->setVisible(visible);
	}
}

void ScrollBarScrollArea::sltVerticalValueChanged(int value)
{
	this->verticalScrollBar()->setValue(value);
}

void ScrollBarScrollArea::sltHorizontalValueChanged(int value)
{
	this->horizontalScrollBar()->setValue(value);
}

void ScrollBarScrollArea::resizeEvent(QResizeEvent * event)
{
	if (m_verticalScrollBar) {
		auto posW = m_verticalScrollBar->width();
		auto posX = this->width() - posW;
		m_verticalScrollBar->setGeometry(posX, 1, posW, this->height() - 2);
	}
	if (m_horizontalScrollBar) {
		auto posH = m_verticalScrollBar->width();
		int posY = this->height() - posH;
		m_horizontalScrollBar->setGeometry(1, posY, this->height() - 2, posH);
	}
	QScrollArea::resizeEvent(event);
}
