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
#include "fonts.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QFile>
#include <QObject>
#include <QtAssert>
#if defined(__MINGW32__) || defined(__MINGW64__)
#  include <QLibrary>
#endif // __MINGW32__ || __MINGW64__

#include <rfl/json.hpp>
#include <SingleApplication>

using namespace Qt::Literals::StringLiterals;

const QString CONFIG_FILENAME{ u"/conf.json"_s };

Util::Config appConfig; // Global configuration hold

#if defined(__MINGW32__) || defined(__MINGW64__)
// Loader for crash helper: Dr.MinGW
[[nodiscard]] inline bool loadExcHndl()
{
    // Load the DLL
    QLibrary *exchndl{ new QLibrary(u"exchndl.dll"_s) };
    Q_ASSERT(exchndl);

    if (exchndl->load()) {
        typedef void (*ProtoExcHndlInit)();
        typedef bool (*ProtoExcHndlSetLogFileNameA)(const char *);
        ProtoExcHndlInit excHndlInit{ reinterpret_cast<ProtoExcHndlInit>(
                exchndl->resolve("ExcHndlInit")) };
        ProtoExcHndlSetLogFileNameA excHndlSetLogFileNameA{
            reinterpret_cast<ProtoExcHndlSetLogFileNameA>(
                    exchndl->resolve("ExcHndlSetLogFileNameA"))
        };
        if (excHndlInit && excHndlSetLogFileNameA) {
            excHndlInit();
            if (excHndlSetLogFileNameA("crashreport.rpt"))
                return true;
                // Do not free the DLL module
        }
    }
    delete exchndl;
    return false;
}
#endif // __MINGW32__ || __MINGW64__

inline void loadTranslations(QApplication *a)
{
    Q_ASSERT(a);
    QTranslator *qtTranslator{ new QTranslator() }, *appTranslator{ new QTranslator() };
    if (qtTranslator->load(QLocale::system(), u"qt"_s, u"_"_s, Util::getTranslationsDir()))
        a->installTranslator(qtTranslator);

    if (appTranslator->load(QLocale::system(), fpsAppName, u"_"_s, Util::getTranslationsDir()))
        a->installTranslator(appTranslator);
    // Do not free translators.
}

[[nodiscard]] inline bool loadConfigurations()
{
    QFile cfgFile(Util::getDataDir() + CONFIG_FILENAME);
    QString jsonCfgStr;
    if (cfgFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream ts(&cfgFile);
        jsonCfgStr = ts.readAll();
    } else {
        QMessageBox::critical(nullptr, fpsAppName,
                              QObject::tr("Error creating/opening configuration file."),
                              QMessageBox::Close);
        return false;
    }

    // First run?
    if (jsonCfgStr.isEmpty())
        Util::setDefConf(appConfig);
    else {
        const auto result{ rfl::json::read<Util::Config>(jsonCfgStr.toStdString()) };
        if (!result) {
            QMessageBox::critical(nullptr, fpsAppName,
                                  QObject::tr("Error parsing configuration file: %1.")
                                          .arg(QString::fromStdString(result.error().what())),
                                  QMessageBox::Close);
            return false;
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
            return false;
        }
    }
    return true;
}

[[nodiscard]] inline bool configureApplication(QApplication *a)
{
    Q_ASSERT(a);
#if defined(__MINGW32__) || defined(__MINGW64__)
    if (!loadExcHndl()) {
        QMessageBox::critical(nullptr, fpsAppName,
                              QObject::tr("Error loading module: exchndl.dll."),
                              QMessageBox::Close);
        return false;
    }
#endif // __MINGW32__ || __MINGW64__
    Util::setAppFont(QLocale::system(), a);

    if (!loadConfigurations())
        return false;

    // Load styles
    if (!Util::setAppSkin(a, QString::fromStdString(appConfig.app.style))) {
        QMessageBox::warning(nullptr, fpsAppName,
                             QObject::tr("Error loading skin: %1.").arg(appConfig.app.style),
                             QMessageBox::Close);
        return false;
    }
    qInfo("Skin loaded.");
    return true;
}

[[nodiscard]] inline bool saveConfigurations()
{
    // Save configuration changes to file
    const QString jsonCfgStr{ QString::fromStdString(rfl::json::write(appConfig)) };
    QFile cfgFile(Util::getDataDir() + CONFIG_FILENAME);
    if (!cfgFile.open(QIODevice::WriteOnly | QFile::Truncate | QIODevice::Text)) {
        QMessageBox::warning(nullptr, fpsAppName,
                             QObject::tr("Error writing to configuration file."),
                             QMessageBox::Close);
        return false;
    }
    QTextStream ts(&cfgFile);
    ts << jsonCfgStr;
    cfgFile.close();
    return true;
}

//---------- Main Execution ----------
int main(int argc, char *argv[])
{
    qSetMessagePattern(u"%{appname} [%{type}] in %{file}:%{line}: %{message}"_s);
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
            Qt::HighDpiScaleFactorRoundingPolicy::RoundPreferFloor);

    QCoreApplication::setApplicationVersion(fpsVersionFull);
    QCoreApplication::setApplicationName(fpsAppName);
    QCoreApplication::setOrganizationName(u"zxunge"_s);
    QGuiApplication::setApplicationDisplayName(fpsAppName);

    SingleApplication a(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(u":/icons/fps.ico"_s));

    if (!configureApplication(&a))
        return -1;

    qInfo("---------------");
    qInfo("Application Started...");
    fpsMainWindow w;
    w.show();
    int ret{ a.exec() };
    if (!saveConfigurations())
        return -1;

    qInfo("Application has finished successfully.");
    qInfo("---------------");
    return ret;
}
