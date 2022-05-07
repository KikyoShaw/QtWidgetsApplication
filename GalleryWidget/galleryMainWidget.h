#pragma once

#include <QWidget>
#include "ui_galleryMainWidget.h"

class GalleryMainWidget : public QWidget
{
	Q_OBJECT
public:
	explicit GalleryMainWidget(QWidget *parent = Q_NULLPTR);
	~GalleryMainWidget();

private:
	Ui::MainWidget ui;
};
