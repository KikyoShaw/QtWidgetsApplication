#include "ColorPickerDemo.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication application(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("ColorPicker"));

    ColorPickerDemo demo;
    demo.show();
    return application.exec();
}