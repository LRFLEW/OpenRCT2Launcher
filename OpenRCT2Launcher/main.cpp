#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("OpenRCT2");
    QCoreApplication::setOrganizationDomain("OpenRCT2.org");
    QCoreApplication::setApplicationName("OpenRCT2Launcher");

    MainWindow w;
    w.show();

    return a.exec();
}
