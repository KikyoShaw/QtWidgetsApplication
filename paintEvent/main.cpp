#include "paintEvent.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    paintEvent w;
    w.show();
    return a.exec();
}
