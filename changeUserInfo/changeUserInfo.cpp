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
		//������ʽ������������paddingΪ16
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
	//���ֻ��8��ͼ
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
		//�����ı�
		m_labelToastAuto->setToast(text);
		//����λ��
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
	qInfo() << QStringLiteral("�޸ĸ�������ҳ: �ϴ�����ͼƬ");
	//���ļ����е�ͼƬ�ļ�
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
					showToast(QStringLiteral("���ϴ���ͼƬ�Ѿ��ظ�����ѡ������ͼƬ"));
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
			showToast(QStringLiteral("ͼƬ�����쳣�������ļ��Ƿ�����"));
		}
	}
}

void changeUserInfo::sltDeleteLogo(QPixmap nPixMap, vLogoItem * obj)
{
	//���ٱ���һ��
	auto oldSize = m_pixMapVec.size();
	if (1 == oldSize) {
		showToast(QStringLiteral("���ٱ���һ��ͼƬ"));
		return;
	}
	if (obj == Q_NULLPTR) {
		qInfo() << __FUNCTION__ << QStringLiteral("���ڶ��󲻴���");
		return;
	}
	if (!m_listItemWidgets.contains(obj)) {
		qInfo() << __FUNCTION__ << QStringLiteral("���ڹ������������ڸô��ڶ���");
		return;
	}

	if (nPixMap.isNull()) {
		qInfo() << __FUNCTION__ << QStringLiteral("ͼƬ����Ϊ��");
		return;
	}

	//�Ƴ�ͼƬ����
	if (m_pixMapVec.contains(nPixMap)) {
		m_pixMapVec.removeOne(nPixMap);
	}

	//�Ƴ�����
	m_listItemWidgets.removeOne(obj);
	ui.horizontalLayout->removeWidget(obj);
	obj->deleteLater();
	auto _size = m_pixMapVec.size();
	//���⴦��ɾ���ڰ���ͼƬ
	if ((MAXIMGNUM - 1) == _size) {
		initAddLogoButton();
	}
}

void changeUserInfo::mouseMoveEvent(QMouseEvent * event)
{
	//�ж�����Ƿ񱻰��£�ֻ����������ˣ��䷵��ֵ����1(true)
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
