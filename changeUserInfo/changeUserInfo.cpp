#include "changeUserInfo.h"
#include <QMouseEvent>
#include "logoItem.h"
#include <QDebug>
#include <QImageReader>
#include <QFileDialog>
#include "LabelToast.h"

#define MAXIMGNUM 8

changeUserInfo::changeUserInfo(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);
	setAttribute(Qt::WA_TranslucentBackground);

	initToast();
	initAddLogoButton();

	connect(ui.pushButton_min, &QPushButton::clicked, this, &changeUserInfo::showMinimized);
	connect(ui.pushButton_close, &QPushButton::clicked, this, &changeUserInfo::close);
}

changeUserInfo::~changeUserInfo()
{
	clearLogoItem();
}

void changeUserInfo::initToast()
{
	m_labelToastAuto = new LabelToastAuto(this);
	if (m_labelToastAuto) {
		m_labelToastAuto->setBackgroundStyle(
			"color:white;"
			"background-color: rgba(0, 0, 0, 70%);"
			"border-radius:8px;"
			"font: 14px Microsoft YaHei;"
			"padding-left:16px;"
			"padding-right:16px;"
		);
		//由于样式中设置了左右padding为16
		m_labelToastAuto->setSpace(16);
		m_labelToastAuto->setFixedHeight(36);
		m_labelToastAuto->hide();
	}
}

void changeUserInfo::initAddLogoButton()
{
	vLogoItem *item = new vLogoItem();
	if (item) {
		connect(item, &vLogoItem::sigAddLogo, this, &changeUserInfo::sltReadLocalImage);
		connect(item, &vLogoItem::sigDeleteLogo, this, &changeUserInfo::sltDeleteLogo);
		ui.horizontalLayout->addWidget(item);
		m_listItemWidgets.append(item);
	}
}

void changeUserInfo::addLogoWidget(QPixmap imgPixMap)
{
	for (auto & each : m_listItemWidgets) {
		if (!each->getImagePixMapState()) {
			each->setImagePixMap(imgPixMap);
		}
	}
	//最多只能8张图
	auto _size = m_pixMapVec.size();
	if (_size > (MAXIMGNUM - 1)) {
		return;
	}
	vLogoItem *item = new vLogoItem();
	if (item) {
		connect(item, &vLogoItem::sigAddLogo, this, &changeUserInfo::sltReadLocalImage);
		connect(item, &vLogoItem::sigDeleteLogo, this, &changeUserInfo::sltDeleteLogo);
		ui.horizontalLayout->addWidget(item);
		m_listItemWidgets.append(item);
	}
}

void changeUserInfo::showToast(const QString & text)
{
	if (m_labelToastAuto) {
		//设置文本
		m_labelToastAuto->setToast(text);
		//调整位置
		int posX = this->width() - m_labelToastAuto->width();
		int posY = this->height() - m_labelToastAuto->height();
		m_labelToastAuto->move(posX / 2, posY / 2);
	}
}

void changeUserInfo::clearLogoItem()
{
	for (auto & each : m_listItemWidgets){
		ui.horizontalLayout->removeWidget(each);
		each->deleteLater();
	}
	m_listItemWidgets.clear();
}

void changeUserInfo::sltReadLocalImage()
{
	qInfo() << QStringLiteral("修改个人资料页: 上传本地图片");
	//打开文件夹中的图片文件
	auto OpenFile = QFileDialog::getOpenFileName(this,
		"Please choose an image file",
		"",
		"Image Files(*.jpg *.png *.webp);");
	if (!OpenFile.isEmpty()) {
		QImageReader reader(OpenFile);
		reader.setDecideFormatFromContent(true);
		if (reader.canRead()) {
			QImage image = QImage();
			if (reader.read(&image)) {
				auto pixMap = QPixmap::fromImage(image);
				if (m_pixMapVec.contains(pixMap)) {
					showToast(QStringLiteral("您上传的图片已经重复，请选择其他图片"));
					return;
				}
				m_pixMapVec.append(pixMap);
				addLogoWidget(pixMap);
				/*QString localImageMD5 = getFileMd5(OpenFile);
				QByteArray localImageBinary = getBinary(OpenFile);
				m_imgMd5Map.insert(pixMap.cacheKey(), localImageMD5);
				m_imgBinaryMap.insert(pixMap.cacheKey(), localImageBinary);*/
			}
		}
		else {
			showToast(QStringLiteral("图片数据异常，请检查文件是否正常"));
		}
	}
}

void changeUserInfo::sltDeleteLogo(QPixmap nPixMap, vLogoItem * obj)
{
	//至少保留一张
	auto oldSize = m_pixMapVec.size();
	if (1 == oldSize) {
		showToast(QStringLiteral("至少保留一张图片"));
		return;
	}
	if (obj == Q_NULLPTR) {
		qInfo() << __FUNCTION__ << QStringLiteral("窗口对象不存在");
		return;
	}
	if (!m_listItemWidgets.contains(obj)) {
		qInfo() << __FUNCTION__ << QStringLiteral("窗口管理容器不存在该窗口对象");
		return;
	}

	if (nPixMap.isNull()) {
		qInfo() << __FUNCTION__ << QStringLiteral("图片数据为空");
		return;
	}

	//移除图片数据
	if (m_pixMapVec.contains(nPixMap)) {
		m_pixMapVec.removeOne(nPixMap);
	}

	//移除窗口
	m_listItemWidgets.removeOne(obj);
	ui.horizontalLayout->removeWidget(obj);
	obj->deleteLater();
	auto _size = m_pixMapVec.size();
	//特殊处理删除第八张图片
	if ((MAXIMGNUM - 1) == _size) {
		initAddLogoButton();
	}
}

void changeUserInfo::mouseMoveEvent(QMouseEvent * event)
{
	//判断左键是否被按下，只有左键按下了，其返回值就是1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void changeUserInfo::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void changeUserInfo::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}
