#include "floatingWin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    floatingWin w;
    w.show();
    return a.exec();
}
