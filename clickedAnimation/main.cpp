#include "clickedAnimation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    clickedAnimation w;
    w.show();
    return a.exec();
}
