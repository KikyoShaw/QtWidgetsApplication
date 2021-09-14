#include "progressTest.h"

progressTest::progressTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	connect(ui.pushButton, &QPushButton::clicked, this, [=]() {
		auto num = ui.progressBar->value();
		num = qMin(num + 1, 100);
		ui.progressBar->setValue(num);
		ui.progressBar_radius->setValue(num);
		ui.progressBar_new->setValue(num);
		ui.progressBar_animation->setValue(num);
		ui.widget->setValue(num);
	});

	connect(ui.pushButton_2, &QPushButton::clicked, this, [=]() {
		auto num = ui.progressBar->value();
		num = qMax(num - 1, 0);
		ui.progressBar->setValue(num);
		ui.progressBar_radius->setValue(num);
		ui.progressBar_new->setValue(num);
		ui.progressBar_animation->setValue(num);
		ui.widget->setValue(num);
	});

	auto value = ui.horizontalSlider->value();
	ui.progressBar->setValue(value);
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.progressBar, &QProgressBar::setValue);

	ui.progressBar_radius->setValue(value);
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.progressBar_radius, &QProgressBar::setValue);

	ui.progressBar_new->setValue(value);
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.progressBar_new, &QProgressBar::setValue);

	ui.progressBar_animation->setValue(value);
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.progressBar_animation, &QProgressBar::setValue);

	ui.widget->setRange(0, 100);
	ui.widget->setValue(value);
	ui.widget->setStyleSheet("background-color: rgb(0, 0, 0);");
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.widget, QOverload<int>::of(&QRoundProgressBar::setValue));
}

progressTest::~progressTest()
{

}
