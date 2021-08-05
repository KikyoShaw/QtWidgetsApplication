#include "carouselLabelTest.h"

carouselLabelTest::carouselLabelTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	//示例代码
	//添加自定义图片和文案描述
	const QList<std::pair<QString, QString>>&& imagepairlst = { std::make_pair(":/image/image/1.jpg", QString::fromLocal8Bit("久未放晴的天空")),\
																std::make_pair(":/image/image/2.jpg", QString::fromLocal8Bit("依旧留着你的笑容")),\
																std::make_pair(":/image/image/3.jpg", QString::fromLocal8Bit("哭过 却无法掩埋歉疚")),
																std::make_pair(":/image/image/4.jpg", QString::fromLocal8Bit("风筝在阴天搁浅")),
																std::make_pair(":/image/image/5.jpg", QString::fromLocal8Bit("想念还在等待救援")), };

	ui.label->addImage(imagepairlst);
}

carouselLabelTest::~carouselLabelTest()
{
}
