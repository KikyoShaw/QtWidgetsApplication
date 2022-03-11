#include "IconPanelTest.h"
#include "iconhelper.h"

IconPanel::IconPanel(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	initPanel1();
	initPanel2();
	initPanel3();
	initPanel4();

	ui.radioButton1->setChecked(true);

	connect(ui.radioButton1, &QRadioButton::toggled, this, &IconPanel::sltToggled1);
	connect(ui.radioButton2, &QRadioButton::toggled, this, &IconPanel::sltToggled2);
	connect(ui.radioButton3, &QRadioButton::toggled, this, &IconPanel::sltToggled3);
	connect(ui.radioButton4, &QRadioButton::toggled, this, &IconPanel::sltToggled4);
}

void IconPanel::initPanel1()
{
	//���ԭ�ж���
	qDeleteAll(labs1);
	labs1.clear();

	//ѡ��ͬ��ͼ������
	int start = 0xe500;
	int end = 0xea5d;
	QFont iconFont = IconHelper::getIconFontAliBaBa();
	IconHelper::setIconFontIndex(-1);

	//���������С
	iconFont.setPixelSize(15);

	//����ͼ���������
	QStringList list;
	for (int icon = start; icon <= end; icon++) {
		QString tip = "0x" + QString::number(icon, 16);
		if (!checkIcon(icon)) {
			list << tip;
			continue;
		}

		QLabel *lab = new QLabel;
		lab->installEventFilter(this);
		lab->setAlignment(Qt::AlignCenter);
		lab->setFont(iconFont);
		lab->setText((QChar)icon);
		lab->setToolTip(tip);
		lab->setMinimumSize(30, 30);
		labs1 << lab;
	}

	//qDebug() << "no text font" << list.count() << list;
	ui.page1->setAutoWidth(true);
	ui.page1->setMargin(3);
	ui.page1->setSpace(3);
	ui.page1->setColumnCount(18);
	ui.page1->setWidgets(labs1);

	//����ͼ��������������ͣʱ��Ч��
	QString qss = QString("QLabel:hover,QLabel:focus{color:%1;border:1px solid %1;}").arg("#00BB9E");
	ui.page1->setStyleSheet(qss);
}

void IconPanel::initPanel2()
{
	//���ԭ�ж���
	qDeleteAll(labs2);
	labs2.clear();

	//ѡ��ͬ��ͼ������
	int start = 0xf000;
	int end = 0xf2e0;
	QFont iconFont = IconHelper::getIconFontAwesome();
	IconHelper::setIconFontIndex(-1);

	//���������С
	iconFont.setPixelSize(15);

	//����ͼ���������
	QStringList list;
	for (int icon = start; icon <= end; icon++) {
		QString tip = "0x" + QString::number(icon, 16);
		if (!checkIcon(icon)) {
			list << tip;
			continue;
		}

		QLabel *lab = new QLabel;
		lab->installEventFilter(this);
		lab->setAlignment(Qt::AlignCenter);
		lab->setFont(iconFont);
		lab->setText((QChar)icon);
		lab->setToolTip(tip);
		lab->setMinimumSize(30, 30);
		labs2 << lab;
	}

	//qDebug() << "no text font" << list.count() << list;
	ui.page2->setAutoWidth(true);
	ui.page2->setMargin(3);
	ui.page2->setSpace(3);
	ui.page2->setColumnCount(18);
	ui.page2->setWidgets(labs2);

	//����ͼ��������������ͣʱ��Ч��
	QString qss = QString("QLabel:hover,QLabel:focus{color:%1;border:1px solid %1;}").arg("#00BB9E");
	ui.page2->setStyleSheet(qss);
}

void IconPanel::initPanel3()
{
	//���ԭ�ж���
	qDeleteAll(labs3);
	labs3.clear();

	//ѡ��ͬ��ͼ������
	int start = 0xe000;
	int end = 0xf8ff;
	QFont iconFont = IconHelper::getIconFontAwesome6();
	IconHelper::setIconFontIndex(2);

	//���������С
	iconFont.setPixelSize(15);

	//����ͼ���������
	QStringList list;
	for (int icon = start; icon <= end; icon++) {
		QString tip = "0x" + QString::number(icon, 16);
		if (!checkIcon(icon)) {
			list << tip;
			continue;
		}

		QLabel *lab = new QLabel;
		lab->installEventFilter(this);
		lab->setAlignment(Qt::AlignCenter);
		lab->setFont(iconFont);
		lab->setText((QChar)icon);
		lab->setToolTip(tip);
		lab->setMinimumSize(30, 30);
		labs3 << lab;
	}

	//qDebug() << "no text font" << list.count() << list;
	ui.page3->setAutoWidth(true);
	ui.page3->setMargin(3);
	ui.page3->setSpace(3);
	ui.page3->setColumnCount(18);
	ui.page3->setWidgets(labs3);

	//����ͼ��������������ͣʱ��Ч��
	QString qss = QString("QLabel:hover,QLabel:focus{color:%1;border:1px solid %1;}").arg("#00BB9E");
	ui.page3->setStyleSheet(qss);
}

void IconPanel::initPanel4()
{
	//���ԭ�ж���
	qDeleteAll(labs4);
	labs4.clear();

	//ѡ��ͬ��ͼ������
	int start = 0xe900;
	int end = 0xe9cf;
	QFont iconFont = IconHelper::getIconFontWeather();
	IconHelper::setIconFontIndex(3);

	//���������С
	iconFont.setPixelSize(15);

	//����ͼ���������
	QStringList list;
	for (int icon = start; icon <= end; icon++) {
		QString tip = "0x" + QString::number(icon, 16);
		if (!checkIcon(icon)) {
			list << tip;
			continue;
		}

		QLabel *lab = new QLabel;
		lab->installEventFilter(this);
		lab->setAlignment(Qt::AlignCenter);
		lab->setFont(iconFont);
		lab->setText((QChar)icon);
		lab->setToolTip(tip);
		lab->setMinimumSize(30, 30);
		labs4 << lab;
	}

	//qDebug() << "no text font" << list.count() << list;
	ui.page4->setAutoWidth(true);
	ui.page4->setMargin(3);
	ui.page4->setSpace(3);
	ui.page4->setColumnCount(18);
	ui.page4->setWidgets(labs4);

	//����ͼ��������������ͣʱ��Ч��
	QString qss = QString("QLabel:hover,QLabel:focus{color:%1;border:1px solid %1;}").arg("#00BB9E");
	ui.page4->setStyleSheet(qss);
}

bool IconPanel::checkIcon(int icon)
{
	//��ͼ�������Ӧֵ����һ��ָ����ɫ��ͼƬ
	QPixmap pix = IconHelper::getPixmap("#FF0000", icon, 120, 120, 120);
	QImage img = pix.toImage();
	int width = img.width();
	int height = img.height();

	//���˲����ڵ�ͼ������
	//�Ը�ͼƬ���ɨ�����ص�,���ǿհ�����ζ�ŵ�ǰͼ�����岻����
	for (int i = 0; i < height; i++) {
		uchar *lineByte = img.scanLine(i);
		for (int j = 0; j < width; j++) {
			uchar tp = lineByte[j];
			if (tp > 0x00) {
				return true;
			}
		}
	}

	//������ͼƬ����
	//QString fileName = QString("%1/icon/%2.jpg").arg(qApp->applicationDirPath()).arg(icon);
	//pix.save(fileName, "jpg");
	return false;
}

void IconPanel::sltToggled2(bool check)
{
	ui.stackedWidget->setCurrentWidget(ui.page2);
}

void IconPanel::sltToggled3(bool check)
{
	ui.stackedWidget->setCurrentWidget(ui.page4);
}

void IconPanel::sltToggled4(bool check)
{
	ui.stackedWidget->setCurrentWidget(ui.page3);
}

void IconPanel::sltToggled1(bool check)
{
	ui.stackedWidget->setCurrentWidget(ui.page1);
}
