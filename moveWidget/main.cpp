#include "moveWidgetTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	moveWidgetTest w;
    w.show();
    return a.exec();
}
