#include "ComboBoxTest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    ComboBoxTest w;
    w.show();
    return a.exec();
}
