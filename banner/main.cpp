#include "bannerTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bannerTest w;
    w.show();
    return a.exec();
}
