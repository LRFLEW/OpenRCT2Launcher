#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class Updater : public QObject
{
    Q_OBJECT
public:
    explicit Updater(QObject *parent = 0);

signals:
    void installed();
    void error(QString error);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

public slots:
    void download();

private:
    void receivedUpdate(QNetworkReply *reply);
    void receivedAPI(QNetworkReply *reply);
    void receivedBundle(QNetworkReply *reply);

private:
    bool extract(QByteArray &data, QDir &bin);

    QUrl url;
    QNetworkAccessManager net;
    int size;
    QByteArray hash;
    QByteArray githash;
    QString version;
};

#endif // UPDATER_H
