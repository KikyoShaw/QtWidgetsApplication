#include "accountitem.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QPixmap>
#include <QPainter>

AccountItem::AccountItem(QWidget *parent)
:QWidget(parent)
{
	m_mousePress = false;
	m_accountImage = new QLabel();
	m_accountImage->setFixedSize(42, 42);
	m_accountNumber = new QLabel();
	m_deleteButton = new QToolButton();
	m_deleteButton->setFixedSize(20, 20);
	m_deleteButton->setIcon(QIcon(":/ComboBoxTest/qrc/ico_gb_list.png"));
	m_deleteButton->setStyleSheet("background-color:transparent;");
	m_deleteButton->setToolTip(QStringLiteral("删除账号信息"));

	connect(m_deleteButton, &QToolButton::clicked, this, &AccountItem::onRemoveAccount);

	// 布局;
	QHBoxLayout *mainLayout = new QHBoxLayout(this);
	mainLayout->addWidget(m_accountImage);
	mainLayout->addWidget(m_accountNumber);
	mainLayout->addStretch();
	mainLayout->addWidget(m_deleteButton);
	mainLayout->setContentsMargins(24, 8, 20, 8);
	mainLayout->setSpacing(20);
	this->setLayout(mainLayout);

	this->setCursor(Qt::PointingHandCursor);
	setFixedWidth(360);
}

AccountItem::~AccountItem()
{

}

// 设置登录用户信息;
void AccountItem::setAccountInfo(int index, const userInfo& info)
{
	m_index = index;
	m_accountUserId = info.userId;
	QFontMetrics fm(m_accountNumber->font());
	QString strElidedText = fm.elidedText(info.nickName, Qt::ElideRight, 130, Qt::TextShowMnemonic);
	QString accountName = QStringLiteral("%1 (%2)").arg(info.userId).arg(strElidedText);
	//设置用户名+用户Id
	m_accountNumber->setText(accountName);
	//设置用户头像
	QPixmap pixmap;
	if(!info.path.isEmpty())
		pixmap.load(info.path);
	if (!pixmap.isNull())
	{
		pixmap = generatePixmap(pixmap, QSize(42, 42), 42);
		m_accountImage->setPixmap(pixmap);
	}
}

// 获取登录用户名称;
QString AccountItem::getAccountName()
{
	return m_accountNumber->text();
}

// 获取当前itemWidget的index;
int AccountItem::getItemWidgetIndex()
{
	return m_index;
}

void AccountItem::hideDeleButton()
{
	m_deleteButton->setVisible(false);
}

void AccountItem::onRemoveAccount()
{
	emit signalRemoveAccount(m_index, m_accountUserId);
}

void AccountItem::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_mousePress = true;
	}
	QWidget::mousePressEvent(event);
}

void AccountItem::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_mousePress)
	{
		emit signalShowAccountInfo(m_index, m_accountUserId);
		m_mousePress = false;
	}
	QWidget::mouseReleaseEvent(event);
}

QPixmap AccountItem::scaledPixmap(const QPixmap& src, int width, int height)
{
	return src.scaled(width, (height == 0 ? width : height),
		Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QPixmap AccountItem::generatePixmap(const QPixmap& src, const QSize& size, const int& radius)
{
	// 无效图片不处理
	if (src.isNull()) {
		return src;
	}

	//获取图片尺寸
	int imageWidth = size.width();
	int imageHeight = size.height();

	// 缩放图片
	QPixmap pixmap = scaledPixmap(src, imageWidth, imageHeight);

	QPixmap destImage(imageWidth, imageHeight);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	// 抗锯齿 + 平滑边缘处理
	painter.setRenderHints(QPainter::Antialiasing, true);
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

	// 裁剪为圆角
	QPainterPath path;
	QRect rect(0, 0, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius);
	painter.setClipPath(path);
	painter.drawPixmap(0, 0, imageWidth, imageHeight, pixmap);
	return destImage;
}