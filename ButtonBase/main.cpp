#include "ButtonTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ButtonBase w;
    w.show();
    return a.exec();
}
