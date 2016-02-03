#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "updater.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void launch();
    void on_optionsButton_clicked();

private:
    Ui::MainWindow *ui;
    Updater updater;
};

#endif // MAINWINDOW_H
