#include "SysteamTrayTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	qApp->setQuitOnLastWindowClosed(false);
    SysteamTrayTest w;
    w.show();
    return a.exec();
}
