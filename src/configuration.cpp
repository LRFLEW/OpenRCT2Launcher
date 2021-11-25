#include "configuration.h"
#include "ui_configuration.h"
#include "configuration_data.h"
#include "platform.h"

#include <QFileDialog>

Configuration::Configuration(QSettings *mainSettings, QString file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuration),
    config(file, QSettings::IniFormat),
    mainSettings(mainSettings)
{
    ui->setupUi(this);
    setComboBoxData();

    ui->launcherVersionMsg->setText(tr("Launcher Version: ") + QStringLiteral(APP_VERSION OPENRCT2_BIT_SUFFIX));

    QVariant hash = mainSettings->value(QStringLiteral("gitHash"));
    if (hash.isValid()) ui->buildHashMsg->setText(tr("OpenRCT2 Git Hash: ") + hash.toByteArray().left(4).toHex().left(7));

    QVariant stable = mainSettings->value(QStringLiteral("stable"));
    if (stable.isValid() && stable.toBool()) ui->stableButton->setChecked(true);

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

    QVariant stableVar = mainSettings->value(QStringLiteral("stable"));
    bool stable = stableVar.isValid() && stableVar.toBool();
    bool newStable = ui->stableButton->isChecked();
    if (stable != newStable) {
        mainSettings->setValue(QStringLiteral("stable"), newStable);
        mainSettings->sync();
        emit redownload();
    }

    delete ui;
}

void Configuration::on_locateRCT2_clicked() {
    QString rct2 =  QFileDialog::getExistingDirectory(this, tr("Select RCT2 Install Location"),
                                                      OPENRCT2_HOMEPATH, QFileDialog::ShowDirsOnly);
    if (!rct2.isEmpty()) ui->rct2Path->setText(rct2);
}
