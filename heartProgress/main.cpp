#include "heartProgress.h"
#include "WaterProgressBarTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    heartProgress w;
	/*WaterProgressBarTest w;*/
    w.show();
    return a.exec();
}
