#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QProcess>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>

#if defined(Q_OS_WIN)
#define OPENRCT2_EXEC_LOCATION "Documents/OpenRCT2/bin/openrct2.exe"
#elif defined(Q_OS_OSX)
#define OPENRCT2_EXEC_LOCATION "Library/Application Support/OpenRCT2/bin/OpenRCT2.app/Contents/MacOS/OpenRCT2"
#elif defined(Q_OS_LINUX)
#define OPENRCT2_EXEC_LOCATION ".cache/OpenRCT2/bin/openrct2"
#else
#error Unsupported Platform
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
    // Prevents showing the "downloaded from the internet" warning
    // Will be moved when downloading is implemented
    QProcess clearattr;
    clearattr.setWorkingDirectory(QDir::homePath());
    clearattr.start("xattr", {"-d", "com.apple.quarantine", "Library/Application Support/OpenRCT2/OpenRCT2.app"});
    clearattr.waitForFinished();
#endif

    QProcess::startDetached(OPENRCT2_EXEC_LOCATION, QStringList(), QDir::homePath());
    QApplication::quit();
}
