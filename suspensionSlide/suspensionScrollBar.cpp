#include "suspensionScrollBar.h"
#include <QFile>
#include <QDebug>

SuspensionScrollBar::SuspensionScrollBar(Qt::Orientation type /* = Qt::Vertical */, QWidget *parent /* = Q_NULLPTR */)
	:QScrollBar(parent)
{
	//Ĭ�ϴ�ֱ������
	this->setOrientation(type);
	//Ĭ�ϳ���
	this->setRange(0, 0);
	//Ĭ����ʽ
	auto path = ":/qss/qrc/qss/whiteScrollbar.qss";
	QFile QSS(path);
	if (QSS.open(QFile::ReadOnly)) {
		QString style(QSS.readAll());
		this->setStyleSheet(style);
	}
	//��ʼ״̬���ػ�����
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
	//�����󳤶ȶ�������0����ô������������
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
	//���û����������ػ���,����հ���ʾ
	this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	//�رտؼ���������
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//��ʼ����ֱ��������
	m_verticalScrollBar = new SuspensionScrollBar(Qt::Vertical, this);
	if (m_verticalScrollBar) {
		//���°󶨻������¼�
		connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), m_verticalScrollBar, SLOT(sltValueChanged(int)));
		connect(m_verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sltHorizontalValueChanged(int)));
		connect(this->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), m_verticalScrollBar, SLOT(sltRangeChanged(int, int)));
	}
	//��ʼ��ˮƽ��������
	m_horizontalScrollBar = new SuspensionScrollBar(Qt::Horizontal, this);
	if (m_horizontalScrollBar) {
		//���°󶨻������¼�
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
	//���û����������ػ���,����հ���ʾ
	this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	//�رտؼ���������
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//��ʼ����ֱ��������
	m_verticalScrollBar = new SuspensionScrollBar(Qt::Vertical, this);
	if (m_verticalScrollBar) {
		//���°󶨻������¼�
		connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), m_verticalScrollBar, SLOT(sltValueChanged(int)));
		connect(m_verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sltHorizontalValueChanged(int)));
		connect(this->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), m_verticalScrollBar, SLOT(sltRangeChanged(int, int)));
	}
	//��ʼ��ˮƽ��������
	m_horizontalScrollBar = new SuspensionScrollBar(Qt::Horizontal, this);
	if (m_horizontalScrollBar) {
		//���°󶨻������¼�
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
	//�رտؼ���������
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//��ʼ����ֱ��������
	m_verticalScrollBar = new SuspensionScrollBar(Qt::Vertical, this);
	if (m_verticalScrollBar) {
		//���°󶨻������¼�
		connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), m_verticalScrollBar, SLOT(sltValueChanged(int)));
		connect(m_verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(sltVerticalValueChanged(int)));
		connect(this->verticalScrollBar(), SIGNAL(rangeChanged(int, int)), m_verticalScrollBar, SLOT(sltRangeChanged(int, int)));
	}
	//��ʼ��ˮƽ��������
	m_horizontalScrollBar = new SuspensionScrollBar(Qt::Horizontal, this);
	if (m_horizontalScrollBar) {
		//���°󶨻������¼�
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
