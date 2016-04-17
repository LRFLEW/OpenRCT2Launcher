#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>
#include <QMap>
#include <QSettings>

namespace Ui {
class Configuration;
}

class Configuration : public QDialog
{
    Q_OBJECT

public:
    explicit Configuration(QSettings *mainSettings, QString file, QWidget *parent = 0);
    ~Configuration();

signals:
    void redownload();

public slots:
    void on_locateRCT2_clicked();

private:
    Ui::Configuration *ui;
    void setComboBoxData();
    QSettings config;
    QSettings *mainSettings;
    QMap<QString, QString> langEquiv;
};

#endif // CONFIGURATION_H
