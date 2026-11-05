#include "ScreenCaptureDemo.h"

#include <QApplication>
#include <QCoreApplication>
#include <QString>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("ScreenCapture"));

    ScreenCaptureDemo demo;
    demo.show();
    return app.exec();
}