#include "carouselLabel.h"
#include <QPainter>
#include <QWheelEvent>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPropertyAnimation> 
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>

const int btn_expand_width = 24;//∞¥≈•¿≠…ÏøÌ∂»
const int btn_shrik_width = 6;//∞¥≈• ’ÀıøÌ∂»

CarouselLabel::CarouselLabel(QWidget *parent)
	: QLabel(parent)
	, m_offset(0)
	, m_curIndex(0)
	, m_preIndex(0)
	, m_blefttoright(true)
	, m_sleepFinsh(true)
{
	setMinimumSize(120, 240);

	m_btnGroup = new QButtonGroup(this);
	connect(m_btnGroup, SIGNAL(buttonClicked(int)), SLOT(sltbuttonClicked(int)));

	m_btnParalGroup = new QParallelAnimationGroup(this);
	m_imageAnimation = new QPropertyAnimation(m_btnParalGroup, "");
	m_imageAnimation->setEasingCurve(QEasingCurve::OutCirc);
	m_imageAnimation->setDuration(1000);
	connect(m_imageAnimation, SIGNAL(valueChanged(const QVariant &)), this, SLOT(sltImagevalueChanged(const QVariant &)));

	QSequentialAnimationGroup* sequentialGroup = new QSequentialAnimationGroup(m_btnParalGroup);
	m_btnExpAnimation = new QPropertyAnimation(sequentialGroup, "");
	m_btnExpAnimation->setEasingCurve(QEasingCurve::OutCubic);
	connect(m_btnExpAnimation, SIGNAL(valueChanged(const QVariant &)), this, SLOT(sltBtnExpvalueChanged(const QVariant &)));
	m_btnExpAnimation->setDuration(400);

	m_btnShrikAnimation = new QPropertyAnimation(sequentialGroup, "");
	m_btnShrikAnimation->setEasingCurve(QEasingCurve::OutCubic);
	connect(m_btnShrikAnimation, SIGNAL(valueChanged(const QVariant &)), this, SLOT(sltBtnShrikvalueChanged(const QVariant &)));
	m_btnShrikAnimation->setDuration(400);

	//∞¥≈•«–ªª¥Æ––‘À––
	sequentialGroup->addAnimation(m_btnExpAnimation);
	sequentialGroup->addAnimation(m_btnShrikAnimation);

	//Õº∆¨«–ªª”Î∞¥≈•«–ªª≤¢––‘À––
	m_btnParalGroup->addAnimation(m_imageAnimation);
	m_btnParalGroup->addAnimation(sequentialGroup);

	initControl();
}

CarouselLabel::~CarouselLabel()
{
}

void CarouselLabel::initControl()
{
	m_horizontalLayoutWidget = new QWidget(this);
	m_horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
	m_switchBtnLayout = new QHBoxLayout(m_horizontalLayoutWidget);
	m_switchBtnLayout->setSpacing(12);
	m_switchBtnLayout->setContentsMargins(0, 0, 0, 0);

	m_imageTimer.setInterval(2000);
	connect(&m_imageTimer, &QTimer::timeout, this, &CarouselLabel::sltImageShowTimeOut);
	m_imageTimer.start();

	m_sleepTimer.setInterval(1000);
	connect(&m_sleepTimer, &QTimer::timeout, this, [=]() {
		m_sleepFinsh = true;
		m_sleepTimer.stop();
	});
}

void CarouselLabel::sltImagevalueChanged(const QVariant & variant)
{
	m_offset = variant.toInt();
	update();
}

void CarouselLabel::sltBtnExpvalueChanged(const QVariant & variant)
{
	m_btnGroup->button(m_curIndex)->setFixedWidth(variant.toInt());
}

void CarouselLabel::sltBtnShrikvalueChanged(const QVariant & variant)
{
	for (int index = 0; index < m_imagepathpairlst.size(); index++) {
		if (m_curIndex != index && m_btnGroup->button(index)->width() > btn_shrik_width) {
			m_btnGroup->button(index)->setFixedWidth(variant.toInt());
		}
	}
}

void CarouselLabel::sltbuttonClicked(int index)
{
	if (m_curIndex == index) {
		return;
	}
	if (index < 0) {
		index = m_imagepathpairlst.size() - 1;
	}
	if (index >= m_imagepathpairlst.size()) {
		index = 0;
	}

	m_preIndex = m_curIndex;
	m_curIndex = index;
	//Õº∆¨«–ªª
	if (m_preIndex < m_curIndex) {
		//œÚ◊Ûª¨
		m_imageAnimation->setStartValue(0);
		m_imageAnimation->setEndValue(0 - width());
		m_blefttoright = false;
	}
	else {
		//œÚ”“ª¨
		m_imageAnimation->setStartValue(0);
		m_imageAnimation->setEndValue(width());
		m_blefttoright = true;
	}
	////∞¥≈•«–ªª
	m_btnShrikAnimation->setStartValue(btn_expand_width);
	m_btnShrikAnimation->setEndValue(btn_shrik_width);

	m_btnExpAnimation->setStartValue(btn_shrik_width);
	m_btnExpAnimation->setEndValue(btn_expand_width);

	m_imageTimer.start(2000);
	m_btnParalGroup->start();
}

void CarouselLabel::paintEvent(QPaintEvent *event)
{
	QLabel::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
	QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
	painter.setPen(QColor(210, 210, 210, 200));
	QFont font = painter.font();
	font.setFamily(QString::fromLocal8Bit("Œ¢»Ì—≈∫⁄"));
	font.setBold(true);
	font.setPixelSize(18);
	painter.setFont(font);

	if (m_blefttoright) {
		painter.drawPixmap(m_offset, 0, QPixmap(m_imagepathpairlst.at(m_preIndex).first).scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
		painter.drawPixmap(m_offset - width(), 0, QPixmap(m_imagepathpairlst.at(m_curIndex).first).scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

		painter.drawText(QRect(m_offset + 12, height() - 52, width(), 32), m_imagepathpairlst.at(m_preIndex).second, option);
		painter.drawText(QRect(m_offset - width() + 12, height() - 52, width(), 32), m_imagepathpairlst.at(m_curIndex).second, option);
	}
	else {
		painter.drawPixmap(m_offset, 0, QPixmap(m_imagepathpairlst.at(m_preIndex).first).scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
		painter.drawPixmap(m_offset + width(), 0, QPixmap(m_imagepathpairlst.at(m_curIndex).first).scaled(width(), height(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

		painter.drawText(QRect(m_offset + 12, height() - 52, width(), 32), m_imagepathpairlst.at(m_preIndex).second, option);
		painter.drawText(QRect(m_offset + width() + 12, height() - 52, width(), 32), m_imagepathpairlst.at(m_curIndex).second, option);
	}
}

void CarouselLabel::resizeEvent(QResizeEvent *event)
{
	m_horizontalLayoutWidget->setGeometry(QRect(12, height() - 18, width() - 24, 6));
	QLabel::resizeEvent(event);
}

void CarouselLabel::wheelEvent(QWheelEvent * event)
{
	if (event->delta() > 0) { //…œª¨
		if (m_sleepFinsh) {
			sltbuttonClicked(m_curIndex - 1);
			m_sleepTimer.start();
		}
		m_sleepFinsh = false;
	}
	else {
		if (m_sleepFinsh) {
			sltbuttonClicked(m_curIndex + 1);
			m_sleepTimer.start();
		}
		m_sleepFinsh = false;
	}
	QLabel::wheelEvent(event);
}

void CarouselLabel::addImage(const QList<std::pair<QString, QString>>& imagepathpairlst)
{
	m_imagepathpairlst = imagepathpairlst;
	for (int index = 0; index < imagepathpairlst.size(); index++)
	{
		QPushButton* button = new QPushButton(m_horizontalLayoutWidget);
		button->setCursor(Qt::PointingHandCursor);
		button->setFixedSize(btn_shrik_width, btn_shrik_width);
		button->setStyleSheet("QPushButton{background: #152034;border-width:0 3px;border-radius: 3px;}");
		m_btnGroup->addButton(button, index);
		m_switchBtnLayout->addWidget(button);
	}
	m_switchBtnLayout->addStretch();
	m_btnGroup->button(m_curIndex)->setFixedWidth(btn_expand_width);
	update();
}

void CarouselLabel::sltImageShowTimeOut()
{
	sltbuttonClicked(m_curIndex + 1);
}