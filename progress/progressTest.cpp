#include "progressTest.h"

progressTest::progressTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	connect(ui.pushButton, &QPushButton::clicked, this, [=]() {
		auto num = ui.progressBar->value();
		num = qMin(num + 1, 100);
		ui.progressBar->setValue(num);
	});

	connect(ui.pushButton_2, &QPushButton::clicked, this, [=]() {
		auto num = ui.progressBar->value();
		num = qMax(num - 1, 0);
		ui.progressBar->setValue(num);
	});

	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.progressBar, &QProgressBar::setValue);

}

progressTest::~progressTest()
{

}
