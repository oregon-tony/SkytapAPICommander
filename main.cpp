#include "skywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SkyWindow w;
    w.show();

    return a.exec();
}
