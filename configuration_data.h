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

    ui->currencies->setItemData(0, QStringLiteral("GBP"));
    ui->currencies->setItemData(1, QStringLiteral("USD"));
    ui->currencies->setItemData(2, QStringLiteral("FRF"));
    ui->currencies->setItemData(3, QStringLiteral("DEM"));
    ui->currencies->setItemData(4, QStringLiteral("JPY"));
    ui->currencies->setItemData(5, QStringLiteral("ESP"));
    ui->currencies->setItemData(6, QStringLiteral("ITL"));
    ui->currencies->setItemData(7, QStringLiteral("NLG"));
    ui->currencies->setItemData(8, QStringLiteral("SEK"));
    ui->currencies->setItemData(9, QStringLiteral("EUR"));
    ui->currencies->setItemData(10, QStringLiteral("KRW"));
    ui->currencies->setItemData(11, QStringLiteral("RUB"));
    ui->currencies->setItemData(12, QStringLiteral("CZK"));

    ui->measurements->setItemData(0, QStringLiteral("IMPERIAL"));
    ui->measurements->setItemData(1, QStringLiteral("METRIC"));
    ui->measurements->setItemData(2, QStringLiteral("SI"));

    ui->temperatures->setItemData(0, QStringLiteral("CELSIUS"));
    ui->temperatures->setItemData(1, QStringLiteral("FAHRENHEIT"));

    ui->dates->setItemData(0, QStringLiteral("DD/MM/YY"));
    ui->dates->setItemData(1, QStringLiteral("MM/DD/YY"));
    ui->dates->setItemData(2, QStringLiteral("YY/MM/DD"));
    ui->dates->setItemData(3, QStringLiteral("YY/DD/MM"));

    ui->screenshotFormat->setItemData(0, QStringLiteral("BMP"));
    ui->screenshotFormat->setItemData(1, QStringLiteral("PNG"));

    ui->themes->setItemData(0, QStringLiteral("*RCT1"));
    ui->themes->setItemData(1, QStringLiteral("*RCT2"));

    ui->titleSequence->setItemData(0, QStringLiteral("*RCT1"));
    ui->titleSequence->setItemData(1, QStringLiteral("*RCT1AA"));
    ui->titleSequence->setItemData(2, QStringLiteral("*RCT1AALL"));
    ui->titleSequence->setItemData(3, QStringLiteral("*RCT2"));
    ui->titleSequence->setItemData(4, QStringLiteral("*OPENRCT2"));

    ui->soundDevices->setItemData(0, QStringLiteral(""));

    langEquiv.insert("en-CA", "en-US");
    langEquiv.insert("zh-CN", "zh-Hant");
    langEquiv.insert("zh-TW", "zh-Hans");
}

#endif // CONFIGURATION_DATA_H
