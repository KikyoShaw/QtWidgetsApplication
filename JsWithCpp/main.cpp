#include "JsWithCpp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9999");
    JsWithCpp w;
    w.show();
    return a.exec();
}
