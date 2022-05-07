#include <QApplication>
#include "galleryMainWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GalleryMainWidget w;
    w.show();
    return a.exec();
}
