#include "updater.h"
#include "platform.h"

#ifdef Q_OS_LINUX
#include "archives/gzipreadfilter.h"
#include "archives/tarextractor.h"
#else
#include "archives/zipextractor.h"
#endif

#include <QBuffer>
#include <QCryptographicHash>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QProcess>
#include <QSettings>
#include <QStringBuilder>
#include <QTemporaryFile>
#include <QUrlQuery>

Updater::Updater(QObject *parent) : QObject(parent), url(QStringLiteral("https://openrct2.org/altapi/"))
{
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("command"), QStringLiteral("get-latest-download"));
    query.addQueryItem(QStringLiteral("flavourId"), QStringLiteral(OPENRCT2_FLAVOR));
    query.addQueryItem(QStringLiteral("gitBranch"), QStringLiteral("develop"));

    url.setQuery(query.query());
}

void Updater::download() {
    QNetworkRequest request(url);
    reply = net.get(request);
    connect(reply, &QNetworkReply::finished, this, &Updater::receivedAPI);
    connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &Updater::errorReply);
}

void Updater::receivedAPI() {
    if (reply->error() != QNetworkReply::NoError) {
        errorReply(reply->error());
        return;
    }

    QByteArray data = reply->readAll();
    reply->close();
    reply->deleteLater();
    reply = nullptr;

    QJsonDocument response = QJsonDocument::fromJson(data);
    QJsonObject robj = response.object();
    if (robj[QStringLiteral("error")].toInt() != 0) {
        emit error(QStringLiteral("Server Error:") + robj[QStringLiteral("errorMessage")].toString());
        return;
    }

    QSettings settings;
    QString have = settings.value(QStringLiteral("downloadId")).toString();
    version = robj[QStringLiteral("downloadId")].toString();

    if (have == version && QDir::home().cd(QStringLiteral(OPENRCT2_BIN))) {
        emit installed();
    } else {
        size = robj[QStringLiteral("fileSize")].toInt();
        hash = QByteArray::fromHex(robj[QStringLiteral("fileHash")].toString().toLatin1());
        githash = QByteArray::fromHex(robj[QStringLiteral("gitHash")].toString().toLatin1());

        QNetworkRequest request(robj[QStringLiteral("url")].toString());
        reply = net.get(request);
        connect(reply, &QNetworkReply::finished, this, &Updater::receivedBundle);
        connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &Updater::errorReply);
        connect(reply, &QNetworkReply::downloadProgress, this, &Updater::downloadProgress);
    }
}

void Updater::receivedBundle() {
    if (reply->error() != QNetworkReply::NoError) {
        errorReply(reply->error());
        return;
    }

    QByteArray data = reply->readAll();
    reply->close();
    reply->deleteLater();
    reply = nullptr;

    if (data.size() != size) {
        emit error(QStringLiteral("Invalid File Downloaded"));
        return;
    }

    QByteArray fhash = QCryptographicHash::hash(data, QCryptographicHash::Algorithm::Sha256);
    if (fhash != hash) {
        emit error(QStringLiteral("Invalid File Downloaded"));
        return;
    }

    QDir bin = QDir::home();
    if (bin.cd(QStringLiteral(OPENRCT2_BIN))) {
        bin.removeRecursively();
        bin.mkpath(QStringLiteral("."));
    } else {
        bin.mkpath(QStringLiteral(OPENRCT2_BIN));
        if (!bin.cd(QStringLiteral(OPENRCT2_BIN))) emit error(QStringLiteral("Unable to create bin folder"));
    }

    if (extract(data, bin)) {
        emit installed();

        QSettings settings;
        settings.setValue(QStringLiteral("downloadId"), version);
        settings.setValue(QStringLiteral("gitHash"), githash);
    } else {
        emit error(QStringLiteral("Unable to extract archive"));
    }
}

void Updater::errorReply(QNetworkReply::NetworkError code) {
    emit error(QStringLiteral("Network Error ") % QString(code) % QStringLiteral(": ") % reply->errorString());
}

bool Updater::extract(QByteArray &data, QDir &bin) {
    QBuffer buffer(&data);
#ifdef Q_OS_LINUX
    GZipReadFilter gzip(&buffer);
    return extractTar(&gzip, bin);
#else
    return extractZip(&buffer, bin);
#endif
}
