#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(QStringLiteral("OpenRCT2"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("OpenRCT2.org"));
    QCoreApplication::setApplicationName(QStringLiteral("OpenRCT2Launcher"));

    QTranslator t;
    if (t.load(QStringLiteral(":/Languages/en-US.qm"))) a.installTranslator(&t);

    MainWindow w;
    w.show();

    return a.exec();
}
