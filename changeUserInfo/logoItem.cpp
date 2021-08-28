#include "logoItem.h"
#include "imageTool.h"
#include <QContextMenuEvent>

vLogoItem::vLogoItem(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent), m_ImgPixMap(QPixmap())
{
	ui.setupUi(this);
	//固定整体大小
	setFixedSize(66, 66);
	//设置鼠标穿透
	ui.label_icon->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	//默认隐藏logo控件
	ui.label_logo->setVisible(false);

	//点击添加
	connect(ui.pushButton_addLogo, &QPushButton::clicked, this, &vLogoItem::sigAddLogo);

	initMenu();
}

vLogoItem::~vLogoItem()
{
}

void vLogoItem::setImagePixMap(QPixmap imgPixMap)
{
	if (!imgPixMap.isNull()) {
		m_ImgPixMap = imgPixMap;
		imgPixMap = getRoundRectPixmap(imgPixMap, QSize(ui.label_logo->size()), 4);
		ui.label_logo->setPixmap(imgPixMap);
		ui.label_logo->setVisible(true);
		ui.label_icon->setVisible(false);
	}
}

bool vLogoItem::getImagePixMapState()
{
	return ui.label_logo->isVisible();
}

void vLogoItem::initMenu()
{
	m_menu.setWindowFlags(Qt::FramelessWindowHint | Qt::Popup /*| Qt::Dialog*/);
	m_menu.setAttribute(Qt::WA_TranslucentBackground);
	m_menu.setStyleSheet("QMenu { background-color: #152034; border-radius: 6px; border: 1px solid rgba(0,0,0,0.2);} \
		QMenu::item { font-family: 微软雅黑; font-size: 12px; color: #FFFFFF; height:30px; width:46px; padding-left:20px; margin:1px 1px;}\
		QMenu::item::selected { background-color: rgba(234, 234, 234, 0.06); color: #01EEC3; } \
		QMenu::right-arrow { border: none; } \
	");

	m_actionDelete = m_menu.addAction(QStringLiteral("删除"));
	m_actionPhoto = m_menu.addAction(QStringLiteral("相册"));

	connect(&m_menu, &QMenu::triggered, this, &vLogoItem::sltMenuTrigger);
}

void vLogoItem::contextMenuEvent(QContextMenuEvent * event)
{
	// 显示菜单,只有有图片数据的时候才显示
	if (getImagePixMapState()) {
		m_menu.move(event->globalPos());
		m_menu.show();
	}
	QWidget::contextMenuEvent(event);
}

void vLogoItem::sltMenuTrigger(QAction * pAction)
{
	if (!pAction) {
		return;
	}
	if (pAction == m_actionDelete) {
		emit sigDeleteLogo(m_ImgPixMap, this);
	}
	else if (pAction == m_actionPhoto) {
		emit sigAddLogo();
	}

}
