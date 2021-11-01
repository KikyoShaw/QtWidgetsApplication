#include "Lrc.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Lrc w;
    w.show();
    return a.exec();
}
