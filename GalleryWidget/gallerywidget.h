#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPixmap>
#include <QDebug>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QScrollBar>
#include "galleryphotowidget.h"

class GalleryWidget : public QScrollArea
{
    Q_OBJECT
public:
    GalleryWidget(QWidget *parent = nullptr);

    void loadData(QList<GalleryPhotoData> list);
    void resizeGallery(QPoint emit_pos = QPoint(0, 0));

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void signalPictureClicked(GalleryPhotoWidget* bean);

public slots:

public:
    static int item_spacing_h;
    static int item_spacing_v;

private:
    QList<GalleryPhotoWidget*> widgets;
    QWidget* center_widget;
};

#endif // GALLERYWIDGET_H
