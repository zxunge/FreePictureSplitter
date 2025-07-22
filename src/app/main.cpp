// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsmainwindow.h"
#include "jsonconfigitems.h"
#include "skins.h"
#include "stdpaths.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QFile>
#include <QObject>

#include <rfl/json.hpp>

Util::Config appConfig;

int main(int argc, char *argv[])
{
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
            Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor);

    QApplication a(argc, argv);

    QCoreApplication::setApplicationName(fpsAppName);
    QCoreApplication::setOrganizationName("zxunge (Grit Clef)");
    QGuiApplication::setApplicationDisplayName(fpsAppName);
    QGuiApplication::setWindowIcon(QIcon(":/icons/fps.ico"));

    // Load translations
    QTranslator qtTranslator, appTranslator;
    if (qtTranslator.load(QLocale::system(), "qt", "_", Util::getTranslationsDir()))
        a.installTranslator(&qtTranslator);

    if (appTranslator.load(QLocale::system(), fpsAppName, "_", Util::getTranslationsDir()))
        a.installTranslator(&appTranslator);

    // Load configurations
    QFile cfgFile(Util::getDataDir() + "/conf.json");
    QString jsonCfgStr;
    if (cfgFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream ts(&cfgFile);
        jsonCfgStr = ts.readAll();
    } else {
        QMessageBox::warning(nullptr, fpsAppName,
                             QObject::tr("Error creating/opening configuration file."),
                             QMessageBox::Close);
        QMetaObject::invokeMethod(&a, &QCoreApplication::quit, Qt::QueuedConnection);
    }

    // First run?
    if (jsonCfgStr.isEmpty())
        Util::setDefConf(appConfig);
    else {
        const auto result{ rfl::json::read<Util::Config>(jsonCfgStr.toStdString()) };
        if (!result) {
            QMessageBox::warning(nullptr, fpsAppName,
                                 QObject::tr("Error parsing configuration file: %1.")
                                         .arg(QString::fromStdString(result.error().what())),
                                 QMessageBox::Close);
            QMetaObject::invokeMethod(&a, &QCoreApplication::quit, Qt::QueuedConnection);
        }
        appConfig = result.value();

        // Check for version differences, as they will cause weird compatibility problems.
        // We promise not to change the interfaces when updating the micro version.
        if (appConfig.app.majorVersion != fpsVersionMajor
            || appConfig.app.minorVersion != fpsVersionMinor) {
            QMessageBox::warning(nullptr, fpsAppName,
                                 QObject::tr("Configuration file\'s version doesn\'t match, try "
                                             "deleting it after backuping."),
                                 QMessageBox::Close);
            QMetaObject::invokeMethod(&a, &QCoreApplication::quit, Qt::QueuedConnection);
        }
    }

    // Load styles
    Util::setAppSkin(&a, QString::fromStdString(appConfig.app.style));

    fpsMainWindow w;
    w.show();

    int ret{ a.exec() };

    // Save configuration changes to file
    jsonCfgStr = QString::fromStdString(rfl::json::write(appConfig));
    cfgFile.close();
    if (!cfgFile.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Text)) {
        QMessageBox::warning(nullptr, fpsAppName,
                             QObject::tr("Error writing to configuration file."),
                             QMessageBox::Close);
        QMetaObject::invokeMethod(&a, &QCoreApplication::quit, Qt::QueuedConnection);
    }
    QTextStream ts(&cfgFile);
    ts << jsonCfgStr;
    cfgFile.close();

    return ret;
}
