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
    ui->progressBar->setHidden(true);

    connect(ui->launchButton, &QPushButton::clicked, this, &MainWindow::launch);

    connect(&updater, &Updater::installed, [this]{ ui->progressBar->setHidden(true); ui->launchButton->setEnabled(true); });
    connect(&updater, &Updater::error, [this](QString error){ ui->errorLabel->setText(error); ui->launchButton->setEnabled(true); });
    connect(&updater, &Updater::downloadProgress, [this](qint64 bytesReceived, qint64 bytesTotal){
        ui->progressBar->setHidden(false); ui->progressBar->setMaximum(bytesTotal); ui->progressBar->setValue(bytesReceived); });

    updater.download();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::launch() {
    QProcess::startDetached(OPENRCT2_EXEC_LOCATION, QStringList(), QDir::homePath());
    QApplication::quit();
}
