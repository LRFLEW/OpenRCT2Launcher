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
#include <QTemporaryFile>
#include <QUrlQuery>

Updater::Updater(QObject *parent) : QObject(parent)
{
    url = "https://openrct2.org/altapi/";
    QUrlQuery query;

    query.addQueryItem("command", "get-latest-download");
    query.addQueryItem("flavourId", OPENRCT2_FLAVOR);
    query.addQueryItem("gitBranch", "develop");

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
    if (robj["error"].toInt() != 0) {
        emit error(QString("Server Error:") + robj["errorMessage"].toString());
        return;
    }

    QSettings settings;
    QString have = settings.value("downloadId").toString();
    version = robj["downloadId"].toString();

    if (have == version && QDir::home().cd(OPENRCT2_BIN)) {
        emit installed();
    } else {
        size = robj["fileSize"].toInt();
        hash = QByteArray::fromHex(robj["fileHash"].toString().toLatin1());
        githash = QByteArray::fromHex(robj["gitHash"].toString().toLatin1());

        QNetworkRequest request(robj["url"].toString());
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
        emit error("Invalid File Downloaded");
        return;
    }

    QByteArray fhash = QCryptographicHash::hash(data, QCryptographicHash::Algorithm::Sha256);
    if (fhash != hash) {
        emit error("Invalid File Downloaded");
        return;
    }

    QDir bin = QDir::home();
    if (bin.cd(OPENRCT2_BIN)) {
        bin.removeRecursively();
        bin.mkpath(".");
    } else {
        bin.mkpath(OPENRCT2_BIN);
        if (!bin.cd(OPENRCT2_BIN)) emit error(QStringLiteral("Unable to create bin folder"));
    }

    if (extract(data, bin)) {
        emit installed();

        QSettings settings;
        settings.setValue("downloadId", version);
        settings.setValue("gitHash", githash);
    } else {
        emit error(QStringLiteral("Unable to extract Tar"));
    }
}

void Updater::errorReply(QNetworkReply::NetworkError code) {
    emit error(QStringLiteral("Network Error ") + code + QStringLiteral(": ") + reply->errorString());
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
