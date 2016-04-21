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
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QProcess>
#include <QStringBuilder>
#include <QTemporaryFile>
#include <QUrlQuery>

Updater::Updater(QObject *parent) : QObject(parent), url(QStringLiteral("https://openrct2.org/altapi/"))
{
    connect(&net, &QNetworkAccessManager::sslErrors, [](QNetworkReply * reply, const QList<QSslError> & errors){Q_UNUSED(reply); qDebug() << errors;});
}

void Updater::download() {
    if (update != nullptr) update->abort(), update = nullptr;
    if (api != nullptr) api->abort(), api = nullptr;
    if (bundle != nullptr) bundle->abort(), bundle = nullptr;

    QUrlQuery query;
    query.addQueryItem(QStringLiteral("command"), QStringLiteral("get-latest-download"));
    query.addQueryItem(QStringLiteral("flavourId"), QStringLiteral(OPENRCT2_FLAVOR));

    QVariant stableVar = settings.value(QStringLiteral("stable"));
    bool stable = stableVar.isValid() && stableVar.toBool();
    query.addQueryItem(QStringLiteral("gitBranch"), stable ? QStringLiteral("master") : QStringLiteral("develop"));

    url.setQuery(query.query());

    QNetworkRequest urequest(QStringLiteral("https://api.github.com/repos/LRFLEW/OpenRCT2Launcher/releases/latest"));
    update = net.get(urequest);
    connect(update, &QNetworkReply::finished, this, &Updater::receivedUpdate);

    QNetworkRequest request(url);
    api = net.get(request);
    connect(api, &QNetworkReply::finished, this, &Updater::receivedAPI);
}

void Updater::receivedUpdate() {
    if (update->error() != QNetworkReply::NoError) {
        //emit error(update->errorString());
        return;
    }

    QByteArray data = update->readAll();
    update->close();
    update->deleteLater();
    update = nullptr;

    QJsonDocument response = QJsonDocument::fromJson(data);
    QJsonObject robj = response.object();
    QString tag = robj[QStringLiteral("tag_name")].toString();

    if (tag.startsWith('v')) {
        QStringRef ver = tag.midRef(1);
        if (ver.compare(QLatin1String(APP_VERSION)) > 0) {
            // Show Update Notification
            QMessageBox msg;
            msg.setText(QStringLiteral("<font size=4>") % tr("New Update is Available") % QStringLiteral("</font>"));
            msg.setInformativeText(
                        QStringLiteral("<font size=4 style=\"white-space:nowrap;\" align=center><p>") %
                        tr("There is a new update available for the launcher.") %
                        QStringLiteral("<br />") %
                        tr("It is recomended that you update.") %
                        QStringLiteral("</p><p>") %
                        tr("Installed Version: ") %
                        QStringLiteral(APP_VERSION) %
                        QStringLiteral("<br />") %
                        tr("Latest Version: ") %
                        ver %
                        QStringLiteral("</p><p><a href=\"") %
                        robj[QStringLiteral("html_url")].toString() %
                        QStringLiteral("\">") %
                        tr("Download Page") %
                        QStringLiteral("</a></p></font>"));
            msg.setMinimumWidth(1000);
            msg.exec();
        }
    }
}

void Updater::receivedAPI() {
    if (api->error() != QNetworkReply::NoError) {
        emit error(api->errorString());
        return;
    }

    QByteArray data = api->readAll();
    api->close();
    api->deleteLater();
    api = nullptr;

    QJsonDocument response = QJsonDocument::fromJson(data);
    QJsonObject robj = response.object();
    if (robj[QStringLiteral("error")].toInt() != 0) {
        emit error(robj[QStringLiteral("errorMessage")].toString());
        return;
    }

    QString have = settings.value(QStringLiteral("downloadId")).toString();
    version = robj[QStringLiteral("downloadId")].toString();

    if (have == version && OPENRCT2_HOMEDIR.cd(QStringLiteral(OPENRCT2_BIN))) {
        emit installed();
    } else {
        size = robj[QStringLiteral("fileSize")].toInt();
        hash = QByteArray::fromHex(robj[QStringLiteral("fileHash")].toString().toLatin1());
        githash = QByteArray::fromHex(robj[QStringLiteral("gitHash")].toString().toLatin1());

        QNetworkRequest request(robj[QStringLiteral("url")].toString());
        bundle = net.get(request);
        connect(bundle, &QNetworkReply::finished, this, &Updater::receivedBundle);
        connect(bundle, &QNetworkReply::downloadProgress, this, &Updater::downloadProgress);
    }
}

void Updater::receivedBundle() {
    if (bundle->error() != QNetworkReply::NoError) {
        emit error(bundle->errorString());
        return;
    }

    QByteArray data = bundle->readAll();
    bundle->close();
    bundle->deleteLater();
    bundle = nullptr;

    if (data.size() != size) {
        emit error(tr("Invalid Download"));
        return;
    }

    QByteArray fhash = QCryptographicHash::hash(data, QCryptographicHash::Algorithm::Sha256);
    if (fhash != hash) {
        emit error(tr("Invalid Download"));
        return;
    }

    QDir bin = OPENRCT2_HOMEDIR;
    if (bin.cd(QStringLiteral(OPENRCT2_BIN))) {
        bin.removeRecursively();
        if (!bin.mkpath(QStringLiteral("."))) {
            emit error(tr("bin dir not created"));
            return;
        }
    } else {
        bin.mkpath(QStringLiteral(OPENRCT2_BIN));
        if (!bin.cd(QStringLiteral(OPENRCT2_BIN))) {
            emit error(tr("bin dir not created"));
            return;
        }
    }

    if (extract(data, bin)) {
        emit installed();

        settings.setValue(QStringLiteral("downloadId"), version);
        settings.setValue(QStringLiteral("gitHash"), githash);
        settings.sync();
    } else {
        emit error(tr("Error extracting archive"));
    }
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
