#include "DragObjectTestWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DragObjectTestWidget w;
    w.show();
    return a.exec();
}
