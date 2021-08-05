#include "logoItem.h"
#include "imageTool.h"

LogoItem::LogoItem(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
}

LogoItem::~LogoItem()
{
}

void LogoItem::setLogo(const QString & path)
{
	QPixmap logo(path);
	if (!logo.isNull()) {
		logo = getRoundRectPixmap(logo, QSize(ui.label_logo->size()), 25);
		ui.label_logo->setScaledContents(true);
		ui.label_logo->setPixmap(logo);
	}
}
