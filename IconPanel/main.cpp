#include "IconPanelTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    IconPanel w;
    w.show();
    return a.exec();
}
