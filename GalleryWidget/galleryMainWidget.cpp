#include "galleryMainWidget.h"

GalleryMainWidget::GalleryMainWidget(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent)
{
	ui.setupUi(this);

	QList<GalleryPhotoData> list;
	srand(time(0));
	for (int i = 1; i < 12; i++)
	{
		QString img = QString(":/GalleryWidget/qrc/%1.jpg").arg(i);
		QPixmap pixmap(img);
		QString title = QStringLiteral("��Ů%1��").arg(i);
		QString subTitle = QStringLiteral("��Ů%1��").arg(i);
		GalleryPhotoData data{ pixmap, title, subTitle };
		list.append(data);
	}
	ui.widget->loadData(list);
}

GalleryMainWidget::~GalleryMainWidget()
{
}
