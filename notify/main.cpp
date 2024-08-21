#include "notifyTestWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    notifyTestWidget w;
    w.show();
    return a.exec();
}
