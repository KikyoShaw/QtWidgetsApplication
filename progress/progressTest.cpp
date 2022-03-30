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
		ui.widget_water->setValue(num);
		ui.widget_water_2->setValue(num);
		ui.widget_battery->setValue(num);
		ui.progressBar_pk->setValue(num);
		ui.widget_R->setValue(num);
	});

	connect(ui.pushButton_2, &QPushButton::clicked, this, [=]() {
		auto num = ui.progressBar->value();
		num = qMax(num - 1, 0);
		ui.progressBar->setValue(num);
		ui.progressBar_radius->setValue(num);
		ui.progressBar_new->setValue(num);
		ui.progressBar_animation->setValue(num);
		ui.widget->setValue(num);
		ui.widget_water->setValue(num);
		ui.widget_water_2->setValue(num);
		ui.widget_battery->setValue(num);
		ui.progressBar_pk->setValue(num);
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

	ui.progressBar_pk->setValue(value);
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.progressBar_pk, &QProgressBar::setValue);

	ui.widget->setRange(0, 100);
	ui.widget->setValue(value);
	ui.widget->setStyleSheet("background-color: rgb(0, 0, 0);");
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.widget, QOverload<int>::of(&QRoundProgressBar::setValue));

	ui.widget_water->setRange(0, 100);
	ui.widget_water->setValue(value);
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.widget_water, &vWaterProgressBar::setValue);

	ui.widget_water_2->setRange(0, 100);
	ui.widget_water_2->setValue(value);
	ui.widget_water_2->setPercentStyle(PercentStyle_Type::PercentStyle_Rect);
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.widget_water_2, &vWaterProgressBar::setValue);

	ui.widget_battery->setRange(0, 100);
	ui.widget_battery->setValue(value);
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.widget_battery, QOverload<int>::of(&BatteryProgressBar::setValue));

	ui.widget_R->setBarStyle(RoundProgressBar::BarStyle::StylePie);
	ui.widget_R->setRange(0, 100);
	ui.widget_R->setValue(value);
	ui.widget_R->setStyleSheet("background-color: rgb(0, 0, 0);");
	QGradientStops grandientPoints;
	grandientPoints << QGradientStop(0, QColor(255, 66, 213)) << QGradientStop(1, QColor(255, 66, 213));
	ui.widget_R->setDataColors(grandientPoints);
	connect(ui.horizontalSlider, &QSlider::valueChanged, ui.widget_R, QOverload<int>::of(&RoundProgressBar::setValue));
}

progressTest::~progressTest()
{

}
