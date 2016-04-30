#ifndef CONFIGURATION_DATA_H
#define CONFIGURATION_DATA_H

#include "configuration.h"
#include "ui_configuration.h"

// This can't be done from the UI file, so I'm doing it here
void Configuration::setComboBoxData() {
    ui->languages->setItemData(0, QStringLiteral("en-GB"));
    ui->languages->setItemData(1, QStringLiteral("en-US"));
    ui->languages->setItemData(2, QStringLiteral("de-DE"));
    ui->languages->setItemData(3, QStringLiteral("nl-NL"));
    ui->languages->setItemData(4, QStringLiteral("fr-FR"));
    ui->languages->setItemData(5, QStringLiteral("hu-HU"));
    ui->languages->setItemData(6, QStringLiteral("pl-PL"));
    ui->languages->setItemData(7, QStringLiteral("es-ES"));
    ui->languages->setItemData(8, QStringLiteral("sv-SE"));
    ui->languages->setItemData(9, QStringLiteral("it-IT"));
    ui->languages->setItemData(10, QStringLiteral("pt-BR"));
    ui->languages->setItemData(11, QStringLiteral("zh-Hant"));
    ui->languages->setItemData(12, QStringLiteral("zh-Hans"));
    ui->languages->setItemData(13, QStringLiteral("fi-FI"));
    ui->languages->setItemData(14, QStringLiteral("ko"));
    ui->languages->setItemData(15, QStringLiteral("ru-RU"));
    ui->languages->setItemData(16, QStringLiteral("ca-CA"));
    ui->languages->setItemData(17, QStringLiteral("jp-JP"));

    ui->screenshotFormat->setItemData(0, QStringLiteral("BMP"));
    ui->screenshotFormat->setItemData(1, QStringLiteral("PNG"));

    langEquiv.insert("en-CA", "en-US");
    langEquiv.insert("zh-CN", "zh-Hant");
    langEquiv.insert("zh-TW", "zh-Hans");
}

#endif // CONFIGURATION_DATA_H
