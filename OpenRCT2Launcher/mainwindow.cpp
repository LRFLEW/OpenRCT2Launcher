#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "platform.h"

#include <QDir>
#include <QProcess>
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>

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
