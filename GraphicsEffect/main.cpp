#include "GraphicsEffect.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphicsEffect w;
    w.show();
    return a.exec();
}
