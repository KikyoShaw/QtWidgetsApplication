#include "galleryphotowidget.h"

int GalleryPhotoWidget::fixed_width = 140;
int GalleryPhotoWidget::fixed_height = 240;
int GalleryPhotoWidget::content_width = 100;
int GalleryPhotoWidget::content_height = 200;
int GalleryPhotoWidget::pixmap_width = 100;
int GalleryPhotoWidget::pixmap_height = 150;
QColor GalleryPhotoWidget::title_color = Qt::black;
QColor GalleryPhotoWidget::subTitle_color = Qt::gray;

GalleryPhotoWidget::GalleryPhotoWidget(GalleryPhotoData data, QWidget *parent) : WaterZoomButton("", parent)
{
    setObjectName("GalleryPhotoWidget");
    setNormalColor(Qt::white);
    setHoverColor(Qt::white);
    setChoking(10);
    setRadius(5, 10);

    this->pixmap = data.pixmap;
    this->title = data.title;
    this->subTitle = data.subTitle;

    setFixedSize(fixed_width, fixed_height);

    this->show();
}

void GalleryPhotoWidget::paintEvent(QPaintEvent *event)
{
    WaterZoomButton::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QPainterPath path;

    // 获取图片路径
    int c;
    int r;
    int margin;
    if (!hover_progress)
    {
        c = choking;
        r = radius_x;
        margin = 10;
    }
    else
    {
        c = choking * (1 - getNolinearProg(hover_progress, hovering?FastSlower:SlowFaster));
        r = radius_zoom < 0 ? radius_x :
                              radius_x + (radius_zoom-radius_x) * hover_progress / 100;
        margin = sqrt(125-hover_progress);
    }

    QRect rect(c+margin,c+margin,size().width()-c*2-margin*2,(size().width()-c*2-margin*2)*pixmap_height/pixmap_width);
    path.addRoundedRect(rect, r, r);
    painter.save();
    painter.setClipPath(path, Qt::IntersectClip);
    painter.drawPixmap(rect, pixmap);
    painter.restore();

    // 画文字
    QFontMetrics fm(this->font());
    int line_height = fm.lineSpacing();

    painter.setPen(title_color);
    painter.drawText(QPoint(rect.left(), rect.bottom()+line_height), title);

    painter.setPen(subTitle_color);
    QRect f_rect = fm.boundingRect(QRect(rect.left(),rect.bottom()+line_height*1.5,rect.width(),0),Qt::TextWordWrap,subTitle);
    painter.drawText(f_rect, Qt::TextWordWrap, subTitle);
}
