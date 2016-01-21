#include "updater.h"

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

#if defined(Q_OS_WIN)
#define OPENRCT2_FLAVOR "1"
#define OPENRCT2_BIN "Documents/OpenRCT2/bin/"
#elif defined(Q_OS_OSX)
#define OPENRCT2_FLAVOR "3"
#define OPENRCT2_BIN "Library/Application Support/OpenRCT2/bin/"
#elif defined(Q_OS_LINUX)
#define OPENRCT2_FLAVOR "4"
#define OPENRCT2_BIN ".cache/OpenRCT2/bin/"
#else
#error Unsupported Platform
#endif

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
        hash = QByteArray::fromHex(robj["fileHash"].toString().toLocal8Bit());

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

#if defined(Q_OS_MAC)
    // This is a workaround to get a quick build released.
    // This will be replaced with a library/class to perform this.
    QTemporaryFile tempzip;
    tempzip.open();
    tempzip.write(data);
    tempzip.close();

    QDir bin = QDir::home();
    if (bin.cd("Library/Application Support/OpenRCT2/bin/")) {
        if (bin.exists()) bin.removeRecursively();
        bin.cdUp();
        bin.mkpath("bin");
    } else {
        bin.mkpath("Library/Application Support/OpenRCT2/bin/");
    }

    QProcess unzip;
    unzip.setWorkingDirectory(QDir::homePath());
    unzip.start("unzip", {QFileInfo(tempzip.fileName()).absoluteFilePath(), "-d", "Library/Application Support/OpenRCT2/bin/"});
    unzip.waitForFinished(5000);
    if (unzip.state() != QProcess::NotRunning) {
        unzip.kill();
        emit error("Unable to Install");
        return;
    }
    if (unzip.exitStatus() != QProcess::NormalExit) {
        emit error("Unable to Install");
        return;
    }

    emit installed();
#elif defined(Q_OS_LINUX)
    // Similar situation to the OS X solution
    // Difference is that Linux uses .tar.gz files
    QTemporaryFile tempzip;
    tempzip.open();
    tempzip.write(data);
    tempzip.close();

    QDir bin = QDir::home();
    if (bin.cd(".cache/OpenRCT2/bin/")) {
        if (bin.exists()) bin.removeRecursively();
        bin.cdUp();
        bin.mkpath("bin");
    } else {
        bin.mkpath(".cache/OpenRCT2/bin/");
    }

    QProcess unzip;
    unzip.setWorkingDirectory(QDir::homePath());
    unzip.execute("tar", {"-zxf", QFileInfo(tempzip.fileName()).absoluteFilePath(), "-C", ".cache/OpenRCT2/bin/"});
    unzip.waitForFinished();
    if (unzip.state() != QProcess::NotRunning) {
        unzip.kill();
        emit error("Unable to Install");
        return;
    }
    if (unzip.exitStatus() != QProcess::NormalExit) {
        emit error("Unable to Install");
        return;
    }

    emit installed();
#elif defined(Q_OS_WIN)
    // I don't know the Windows Command-Line, so I can't hack it
    // Just auto-fail for now.
    emit error("Unable to Install");
    return;
#else
#error Unsupported Platform
#endif
    QSettings settings;
    settings.setValue("downloadId", version);
}

void Updater::errorReply(QNetworkReply::NetworkError code) {
    emit error(QString("Network Error ") + code + ": " + reply->errorString());
}
