#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "interactivebuttonbase.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initData()
{
    QList<GalleryPhotoData> list;
    srand(time(0));
    //int count = rand() % 10 + 5;
    for (int i = 1; i < 12; i++)
    {
		QString img = QString(":/GalleryWidget/qrc/%1.jpg").arg(i);
        QPixmap pixmap(img);
        QString title = QStringLiteral("美女%1号").arg(i);
        QString subTitle = QStringLiteral("美女%1号").arg(i);
        GalleryPhotoData data{pixmap, title, subTitle};
        list.append(data);
    }
    ui->widget->loadData(list);
}
