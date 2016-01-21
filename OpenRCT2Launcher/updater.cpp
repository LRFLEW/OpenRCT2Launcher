#include "updater.h"
#include "platform.h"

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

#include <archive.h>
#include <archive_entry.h>

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

    QDir bin = QDir::home();
    if (bin.cd(OPENRCT2_BIN)) {
        bin.removeRecursively();
        bin.mkdir("");
    } else {
        bin.mkpath(OPENRCT2_BIN);
    }

    if (extract(data)) {
        emit installed();

        QSettings settings;
        settings.setValue("downloadId", version);
    }
}

void Updater::errorReply(QNetworkReply::NetworkError code) {
    emit error(QString("Network Error ") + code + ": " + reply->errorString());
}

bool Updater::extract(QByteArray &data) {
    struct archive *a = archive_read_new();

#ifdef Q_OS_LINUX
    archive_read_support_compression_gzip(a);
    archive_read_support_format_tar(a);
#else
    archive_read_support_format_zip(a);
#endif

    int r = archive_read_open_memory(a, data.data(), data.size());
    if (r != ARCHIVE_OK) {
        emit error(archive_error_string(a));
        return false;
    }

    struct archive_entry *entry;
    char buffer[4096];
    QDir bin(QDir::home());
    bin.cd(OPENRCT2_BIN);
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        quint16 type = archive_entry_filetype(entry);
        QString name = archive_entry_pathname(entry);
        if (type == AE_IFDIR) {
            QDir dir(bin);
            dir.mkdir(name);
        } else if (type == AE_IFREG) {
            QFileInfo info(bin, name);
            QFile file(info.absoluteFilePath());
            file.open(QFile::WriteOnly);

            size_t read;
            do {
                read = archive_read_data(a, buffer, 4096);
                file.write(buffer, read);
            } while (read != 0);
            file.close();

            int perm = archive_entry_mode(entry) & ~AE_IFMT;
            file.setPermissions(static_cast<QFile::Permissions>(((perm & 0700) << 6) | ((perm & 0700) << 2) | ((perm & 0070) << 1) | (perm & 0007)));
        } else {
            emit error("Unknown file type");
            return false;
        }
    }

    r = archive_read_finish(a);
    if (r != ARCHIVE_OK) {
        emit error(archive_error_string(a));
        return false;
    }

    return true;
}
