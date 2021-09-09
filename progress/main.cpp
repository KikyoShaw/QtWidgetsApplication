#include "progressTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    progressTest w;
    w.show();
    return a.exec();
}
