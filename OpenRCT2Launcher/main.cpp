#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(QStringLiteral("OpenRCT2"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("OpenRCT2.org"));
    QCoreApplication::setApplicationName(QStringLiteral("OpenRCT2Launcher"));

    MainWindow w;
    w.show();

    return a.exec();
}
