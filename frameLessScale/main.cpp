#include "frameLessScale.h"
#include <QtWidgets/QApplication>
#include "frameless.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    frameLessScale w;
	FrameLess *frameLess = new FrameLess(&w);
    w.show();
    return a.exec();
}
