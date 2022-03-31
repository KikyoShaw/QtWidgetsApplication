#include "gallerywidget.h"

int GalleryWidget::item_spacing_h = 10;
int GalleryWidget::item_spacing_v = 10;

GalleryWidget::GalleryWidget(QWidget *parent) : QScrollArea(parent)
{
    center_widget = new QWidget(this);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidget(center_widget);
}

/**
 * 加载数据，初始化列表
 * 并且开启显示动画
 */
void GalleryWidget::loadData(QList<GalleryPhotoData> list)
{
    foreach (GalleryPhotoWidget* widget, widgets) {
        widget->deleteLater();
    }
    widgets.clear();

    foreach (GalleryPhotoData data, list)
    {
        widgets.append(new GalleryPhotoWidget(data, center_widget));
    }

    resizeGallery();
}

/**
 * 重新调整所有控件的位置
 */
void GalleryWidget::resizeGallery(QPoint emit_pos)
{
    if (widgets.isEmpty())
        return ;
    int gpw_width = GalleryPhotoWidget::fixed_width, gpw_height = GalleryPhotoWidget::fixed_height;
    int bar_width = verticalScrollBar()->width();
    int col_count = qMax((center_widget->width()-item_spacing_h-bar_width) / (gpw_width + item_spacing_h), 1); // 一列数量
    if (col_count > widgets.size())
        col_count = widgets.size();
    int row_count = qMax((widgets.size() + col_count - 1) / col_count, 1); // 行数
    int total_height = row_count * (gpw_height + item_spacing_v) + item_spacing_v*2;
    center_widget->setMinimumHeight(total_height);
    center_widget->resize(center_widget->width(), total_height);
    int total_left = (center_widget->width() - col_count * (gpw_width + item_spacing_h)) / 2;
    int total_top = item_spacing_v;

    int cur_row = 0, cur_col = 0;
    for (int i = 0; i < widgets.size(); i++)
    {
        GalleryPhotoWidget* widget = widgets.at(i);
        QPoint pos(total_left + cur_col * (gpw_width + item_spacing_h), total_top + cur_row * (gpw_height + item_spacing_v));
        QPropertyAnimation* ani = new QPropertyAnimation(widget, "pos");
        ani->setStartValue(widget->pos());
        ani->setEndValue(pos);
        ani->setDuration(300);
        ani->setEasingCurve(QEasingCurve::OutQuad);
        connect(ani, SIGNAL(finished()), ani, SLOT(deleteLater()));
        ani->start();

        cur_col++;
        if (cur_col >= col_count)
        {
            cur_col = 0;
            cur_row++;
        }
    }
}

void GalleryWidget::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);
    center_widget->setFixedWidth(width());
    resizeGallery();
}
