#include "testWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	testWidget w;
    w.show();
    return a.exec();
}
