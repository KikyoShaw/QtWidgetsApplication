#include "animationTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    animationTest w;
    w.show();
    return a.exec();
}
