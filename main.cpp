#include "skywindow.h"
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    //QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
    QApplication a(argc, argv);
    SkyWindow w;
    w.show();

    return a.exec();
}
