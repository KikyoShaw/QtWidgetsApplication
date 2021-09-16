#include "heartProgress.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    heartProgress w;
    w.show();
    return a.exec();
}
