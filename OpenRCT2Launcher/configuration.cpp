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
        if (themesDir.cd(OPENRCT2_BASE "themes")) {
            // remove ini when json is merged
            QFileInfoList themes = themesDir.entryInfoList({"*.ini", "*.json"},
                    QDir::Files | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name);
            for (QFileInfo &info : themes) {
                ui->themes->addItem(info.baseName(), QVariant(info.baseName()));
            }
        }
    }

    {
        QDir themesDir = QDir::home();
        if (themesDir.cd(OPENRCT2_BASE "title sequences")) {
            QFileInfoList themes = themesDir.entryInfoList(
                    QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable, QDir::Name);
            for (QFileInfo &info : themes) {
                if (QFile::exists(QDir(info.filePath()).filePath("script.txt")))
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
    QString rct2 =  QFileDialog::getExistingDirectory(this, QStringLiteral("Select RCT2 Install Location"),
                                                      QDir::homePath(), QFileDialog::ShowDirsOnly);
    if (!rct2.isEmpty()) ui->rct2Path->setText(rct2);
}
