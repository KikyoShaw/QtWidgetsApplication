#include "carouselLabelTest.h"

carouselLabelTest::carouselLabelTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	//ʾ������
	//����Զ���ͼƬ���İ�����
	const QList<std::pair<QString, QString>>&& imagepairlst = { std::make_pair(":/image/image/1.jpg", QString::fromLocal8Bit("��δ��������")),\
																std::make_pair(":/image/image/2.jpg", QString::fromLocal8Bit("�����������Ц��")),\
																std::make_pair(":/image/image/3.jpg", QString::fromLocal8Bit("�޹� ȴ�޷�����Ǹ��")),
																std::make_pair(":/image/image/4.jpg", QString::fromLocal8Bit("�����������ǳ")),
																std::make_pair(":/image/image/5.jpg", QString::fromLocal8Bit("����ڵȴ���Ԯ")), };

	ui.label->addImage(imagepairlst);
}

carouselLabelTest::~carouselLabelTest()
{
}
