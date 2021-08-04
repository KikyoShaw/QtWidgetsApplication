#include "marqueeLabel.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    marqueeLabel w;
    w.show();
    return a.exec();
}
