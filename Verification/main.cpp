#include "VerificationTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	VerificationTest w;
    w.show();
    return a.exec();
}
