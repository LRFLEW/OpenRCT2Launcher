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

private slots:
    void receivedAPI();
    void receivedBundle();
    void errorReply(QNetworkReply::NetworkError code);

private:
    QUrl url;
    QNetworkAccessManager net;
    QNetworkReply *reply;
    int size;
    QByteArray hash;
    QString version;
};

#endif // UPDATER_H
