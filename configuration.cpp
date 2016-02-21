#include "configuration.h"
#include "ui_configuration.h"
#include "configuration_data.h"
#include "platform.h"

#include <QAudioDeviceInfo>
#include <QDir>
#include <QFileDialog>
#include <QScreen>

Configuration::Configuration(QString file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuration),
    config(file, QSettings::IniFormat)
{
    ui->setupUi(this);
    setComboBoxData();

    {
        QSettings main;

        ui->launcherVersionMsg->setText(tr("Launcher Version: ") + QStringLiteral(APP_VERSION));

        QVariant build = main.value(QStringLiteral("downloadId"));
        if (build.isValid()) ui->buildVersionMsg->setText(tr("OpenRCT2 Build: ") + build.toString());

        QVariant hash = main.value(QStringLiteral("gitHash"));
        if (build.isValid()) ui->buildHashMsg->setText(tr("OpenRCT2 Git Hash: ") + hash.toByteArray().left(4).toHex().left(7));
    }

    for (QLineEdit *w : ui->tabWidget->findChildren<QLineEdit *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            QVariant cvalue = config.value(setting.toString());
            if (cvalue.isValid()) w->setText(cvalue.toString());
        }
    }

    for (QCheckBox *w : ui->tabWidget->findChildren<QCheckBox *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            QVariant cvalue = config.value(setting.toString());
            if (cvalue.convert(QMetaType::Bool)) w->setChecked(cvalue.toBool());
        }
    }

    for (QSpinBox *w : ui->tabWidget->findChildren<QSpinBox *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            QVariant cvalue = config.value(setting.toString());
            if (cvalue.convert(QMetaType::Int)) w->setValue(cvalue.toInt());
        }
    }

    for (QDoubleSpinBox *w : ui->tabWidget->findChildren<QDoubleSpinBox *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            QVariant cvalue = config.value(setting.toString());
            if (cvalue.convert(QMetaType::Double)) w->setValue(cvalue.toDouble());
        }
    }

    for (QSlider *w : ui->tabWidget->findChildren<QSlider *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            QVariant cvalue = config.value(setting.toString());
            if (cvalue.convert(QMetaType::Int)) w->setValue(cvalue.toInt());
        }
    }

    {
        QList<QAudioDeviceInfo> outputDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
        for (QAudioDeviceInfo &info : outputDevices) {
            ui->soundDevices->addItem(info.deviceName(), info.deviceName());
        }
    }

    {
        QDir themesDir = QDir::home();
        if (themesDir.cd(QStringLiteral(OPENRCT2_THEMES_LOCATION))) {
            // remove ini when json is merged
            QFileInfoList themes = themesDir.entryInfoList({QStringLiteral("*.ini"), QStringLiteral("*.json")},
                    QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name);
            for (QFileInfo &info : themes) {
                ui->themes->addItem(info.baseName(), QVariant(info.baseName()));
            }
        }
    }

    {
        QDir themesDir = QDir::home();
        if (themesDir.cd(OPENRCT2_TITLE_LOCATION)) {
            QFileInfoList themes = themesDir.entryInfoList(
                    QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name);
            for (QFileInfo &info : themes) {
                if (QFile::exists(QDir(info.filePath()).filePath(QStringLiteral("script.txt"))))
                    ui->titleSequence->addItem(info.baseName(), QVariant(info.baseName()));
            }
        }
    }

    for (QComboBox *w : ui->tabWidget->findChildren<QComboBox *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            QString configName = setting.toString();
            QVariant cvalue = config.value(setting.toString());
            if (cvalue.isValid()) {
                int index = -1;
                QString configValue = cvalue.toString();
                if (cvalue.convert(QMetaType::Int)) {
                    index = cvalue.toInt();
                    QVariant offset = w->property("configOffset");
                    if (offset.isValid()) index -= offset.toInt();
                } else {
                    index = w->findData(configValue);
                }
                if (index >= 0) w->setCurrentIndex(index);
            }
        }
    }

    if (ui->fullscreenWidth->value() == 0 && ui->fullscreenHeight->value() == 0) {
        QSize display = QApplication::primaryScreen()->size();
        ui->fullscreenWidth->setValue(display.width());
        ui->fullscreenHeight->setValue(display.height());
    }

    // Locale Stuff
    {
        QLocale locale;
        if (!config.value(QStringLiteral("language")).isValid()) {
            QString name = locale.name();
            name = name.replace('_', '-');
            int ind = ui->languages->findData(langEquiv.value(name, name), Qt::UserRole, Qt::MatchExactly);

            if (ind < 0) {
                QStringList languages = locale.uiLanguages();
                for (QString lang : languages) {
                    lang = lang.replace('_', '-');
                    ind = ui->languages->findData(langEquiv.value(lang, lang), Qt::UserRole, Qt::MatchExactly);
                    if (ind >= 0) break;
                }
            }

            if (ind < 0) {
                QStringList languages = locale.uiLanguages();
                for (QString lang : languages) {
                    lang = lang.replace('_', '-');
                    if (lang.contains('-')) lang.truncate(lang.indexOf('-'));
                    ind = ui->languages->findData(lang, Qt::UserRole, Qt::MatchStartsWith);
                    if (ind >= 0) break;
                }
            }

            ui->languages->setCurrentIndex(ind);
        }
        if (!config.value(QStringLiteral("currency_format")).isValid()) {
            int ind = ui->currencies->findData(locale.currencySymbol(QLocale::CurrencyIsoCode));
            if (ind >= 0) ui->currencies->setCurrentIndex(ind);
        }
        if (!config.value(QStringLiteral("measurement_format")).isValid()) {
            ui->measurements->setCurrentIndex((locale.measurementSystem() == QLocale::MetricSystem) ? 1 : 0);
        }
        if (!config.value(QStringLiteral("temperature_format")).isValid()) {
            ui->temperatures->setCurrentIndex((locale.measurementSystem() == QLocale::ImperialUSSystem) ? 1 : 0);
        }
        if (!config.value(QStringLiteral("date_format")).isValid()) {
            QString df = locale.dateFormat();
            // dateFormat is not quite what we need, so I'll translate it
            int day = df.indexOf('d', 0, Qt::CaseInsensitive);
            int month = df.indexOf('M', 0, Qt::CaseInsensitive);
            int year = df.indexOf('y', 0, Qt::CaseInsensitive);
            // Assume that if year is not first, it's last
            if (day >= 0 && month >= 0 && year >= 0) {
                if (month < day) {
                    if (year < month) ui->dates->setCurrentIndex(2);
                    else ui->dates->setCurrentIndex(1);
                } else {
                    if (year < day) ui->dates->setCurrentIndex(3);
                    else ui->dates->setCurrentIndex(0);
                }
            }
        }
    }
}

Configuration::~Configuration()
{
    for (QLineEdit *w : ui->tabWidget->findChildren<QLineEdit *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            config.setValue(setting.toString(), w->text());
        }
    }

    for (QCheckBox *w : ui->tabWidget->findChildren<QCheckBox *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            config.setValue(setting.toString(), w->isChecked());
        }
    }

    for (QSpinBox *w : ui->tabWidget->findChildren<QSpinBox *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            config.setValue(setting.toString(), w->value());
        }
    }

    for (QDoubleSpinBox *w : ui->tabWidget->findChildren<QDoubleSpinBox *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            config.setValue(setting.toString(), w->value());
        }
    }

    for (QSlider *w : ui->tabWidget->findChildren<QSlider *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            config.setValue(setting.toString(), w->value());
        }
    }

    for (QComboBox *w : ui->tabWidget->findChildren<QComboBox *>()) {
        QVariant setting = w->property("config");
        if (setting.isValid()) {
            if (w->currentData().isValid()) {
                config.setValue(setting.toString(), w->currentData());
            } else {
                QVariant offset = w->property("configOffset");
                if (offset.isValid()) config.setValue(setting.toString(), w->currentIndex() + offset.toInt());
                else config.setValue(setting.toString(), w->currentIndex());
            }
        }
    }

    delete ui;
}

void Configuration::on_locateRCT2_clicked() {
    QString rct2 =  QFileDialog::getExistingDirectory(this, tr("Select RCT2 Install Location"),
                                                      QDir::homePath(), QFileDialog::ShowDirsOnly);
    if (!rct2.isEmpty()) ui->rct2Path->setText(rct2);
}
