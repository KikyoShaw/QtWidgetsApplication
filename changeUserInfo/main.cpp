#include "changeUserInfo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    changeUserInfo w;
    w.show();
    return a.exec();
}
