#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QUrl>

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = 0);
    QSettings settings;

signals:
    void installed();
    void error(QString error);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

public slots:
    void download();

private slots:
    void receivedUpdate();
    void receivedAPI();
    void receivedBundle();

private:
    bool extract(QByteArray &data, QDir &bin);
    void queryDownloads(QString flavor);

    QNetworkAccessManager net;
    int size;
    QByteArray hash;
    QByteArray githash;
    QString version;

    QNetworkReply *update = nullptr;
    QNetworkReply *api = nullptr;
    QNetworkReply *bundle = nullptr;
};

#endif // UPDATER_H
