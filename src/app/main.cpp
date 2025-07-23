/*
 * This file is a part of FreePictureSplitter, a fast and easy-to-use picture splitter.
 * Copyright (c) 2024 2025 zxunge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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

using namespace Qt::Literals::StringLiterals;

Util::Config appConfig;

int main(int argc, char *argv[])
{
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
            Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor);

    QApplication a(argc, argv);

    QCoreApplication::setApplicationName(fpsAppName);
    QCoreApplication::setOrganizationName(u"zxunge (Grit Clef)"_s);
    QGuiApplication::setApplicationDisplayName(fpsAppName);
    QGuiApplication::setWindowIcon(QIcon(u":/icons/fps.ico"_s));

    // Load translations
    QTranslator qtTranslator, appTranslator;
    if (qtTranslator.load(QLocale::system(), u"qt"_s, u"_"_s, Util::getTranslationsDir()))
        a.installTranslator(&qtTranslator);

    if (appTranslator.load(QLocale::system(), fpsAppName, u"_"_s, Util::getTranslationsDir()))
        a.installTranslator(&appTranslator);

    // Load configurations
    QFile cfgFile(Util::getDataDir() + u"/conf.json"_s);
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
