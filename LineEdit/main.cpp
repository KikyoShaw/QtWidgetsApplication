#include "LineEdit.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LineEdit w;
    w.show();
    return a.exec();
}
