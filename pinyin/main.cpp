#include "pinyinTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pinyinTest w;
    w.show();
    return a.exec();
}
