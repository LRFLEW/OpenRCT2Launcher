#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QProcess>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>

#ifdef Q_OS_OSX
#define OPENRCT2_EXEC "Library/Application Support/OpenRCT2/OpenRCT2.app/Contents/MacOS/OpenRCT2"
#else
#error "Unsupported Platform"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->splashScreen->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    ui->splashScreen->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);

    connect(ui->launchButton, &QPushButton::clicked, this, &MainWindow::launch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::launch() {
#ifdef Q_OS_OSX
    QProcess clearattr;
    clearattr.setWorkingDirectory(QDir::homePath());
    clearattr.start("xattr", {"-d", "com.apple.quarantine", "Library/Application Support/OpenRCT2/OpenRCT2.app"});
    clearattr.waitForFinished();
#endif

    QProcess::startDetached("open", QStringList("Library/Application Support/OpenRCT2/OpenRCT2.app"), QDir::homePath());
    QApplication::quit();
}
