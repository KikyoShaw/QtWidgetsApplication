#include "DockWin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DockWin w;
    w.show();
    return a.exec();
}
