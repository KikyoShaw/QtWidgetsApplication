#include "LabelToast.h"
#include <QAbstractTextDocumentLayout>

LabelToast::LabelToast(QWidget *parent)
	: QLabel(parent)
{
	setAlignment(Qt::AlignCenter);
	setWordWrap(true);
	m_timer.setInterval(2000);
	m_timer.setSingleShot(true);
	connect(&m_timer, &QTimer::timeout, this, &QLabel::close);
}

LabelToast::~LabelToast()
{
}

void LabelToast::setToast(const QString & text)
{
	m_timer.start();
	setText(text);
	show();
}

void LabelToast::closeEvent(QCloseEvent * event)
{
	clear();
	m_timer.stop();
	QLabel::closeEvent(event);
}

LabelToastAuto::LabelToastAuto(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_TransparentForMouseEvents, true);
	//ui.label_toastAuto->setAlignment(Qt::AlignCenter);
	//ui.label_toastAuto->setWordWrap(true);
	m_timer.setInterval(2000);
	m_timer.setSingleShot(true);
	connect(&m_timer, &QTimer::timeout, this, &QLabel::close);
}

LabelToastAuto::~LabelToastAuto()
{
}

void LabelToastAuto::setToast(const QString & text)
{
	m_timer.start();
	ui.label_toastAuto->setText(text);
	//设置大小，方便上层窗口居中显示
	QFontMetrics fm(ui.label_toastAuto->font());
	int textWidth = fm.width(text);
	setFixedWidth(textWidth + m_space * 2 + 6);
	show();
}

void LabelToastAuto::setBackgroundStyle(const QString & style)
{
	ui.label_toastAuto->setStyleSheet(style);
}

void LabelToastAuto::setSpace(int space)
{
	m_space = space;
}

void LabelToastAuto::closeEvent(QCloseEvent * event)
{
	ui.label_toastAuto->clear();
	m_timer.stop();
	QWidget::closeEvent(event);
}


