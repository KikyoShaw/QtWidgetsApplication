#include "chatBubbe.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chatBubbe w;
    w.show();
    return a.exec();
}
