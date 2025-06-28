// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsmainwindow.h"
#include "config.h"
#include "jsonconfigitems.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>
#include <QMessageBox>
#include <QtCore/qfile.h>

#include <rfl/json.hpp>

Util::Config appConfig;

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
            Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif

    QApplication a(argc, argv);
    QCoreApplication::setApplicationName(fpsAppName);
    QCoreApplication::setOrganizationName("zxunge (Grit Clef)");
    QGuiApplication::setApplicationDisplayName(fpsAppName);
    QGuiApplication::setWindowIcon(QIcon(":/icons/fps.ico"));

    // Load configuration
    QFile cfgFile("conf.json");
    QString jsonCfgStr;
    QTextStream ts(&cfgFile);
    if (cfgFile.open(QIODevice::ReadWrite | QIODevice::Text))
        jsonCfgStr = ts.readAll();
    else {
        QMessageBox::warning(nullptr, QStringLiteral("FreePictureSplitter"),
                             QStringLiteral("Error creating/opening configuration file."),
                             QMessageBox::Close);
        QApplication::exit(1);
    }

    // First run?
    if (jsonCfgStr.isEmpty())
        appConfig = Util::Config{ .app = { .name = fpsAppName,
                                           .fullVersion = fpsVersionFull,
                                           .majorVersion = fpsVersionMajor,
                                           .minorVersion = fpsVersionMinor,
                                           .microVersion = fpsVersionMicro,
                                           .style = "default" } };
    else {
        appConfig = rfl::json::read<Util::Config>(jsonCfgStr.toStdString()).value();
        // Check for version differences, as they will cause weird compatibility problems.
        // We promise not to change the interfaces when updating the micro version.
        if (appConfig.app.majorVersion != fpsVersionMajor
            || appConfig.app.minorVersion != fpsVersionMinor) {
            QMessageBox::warning(nullptr, QStringLiteral("FreePictureSplitter"),
                                 QStringLiteral("Configuration file\'s version doesn\'t match, try "
                                                "deleting it after backuping."),
                                 QMessageBox::Close);
            QApplication::exit(1);
        }
    }

    // Load styles
    QFile styleFile;
    if (appConfig.app.style == "default") {
        // The default style includes a fusion style
        a.setStyle(QStyleFactory::create("fusion"));
        styleFile.setFileName(QStringLiteral(":/skins/skins/default.qss"));
    } else
        styleFile.setFileName(QStringLiteral(":/skins/skins/")
                              + QString::fromStdString(appConfig.app.style) + ".qss");

    styleFile.open(QFile::ReadOnly);
    if (styleFile.isOpen()) {
        if (appConfig.app.style == "lightblue") // "lightblue" requires changing the palette
            a.setPalette(QPalette("#eaf7ff"));
        a.setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    } else {
        QMessageBox::warning(nullptr, QStringLiteral("FreePictureSplitter"),
                             QStringLiteral("Error loading skin."), QMessageBox::Close);
        QApplication::exit(1);
    }

    QTranslator translator;
    const QStringList uiLanguages{ QLocale::system().uiLanguages() };
    for (const QString &locale : uiLanguages) {
        const QString baseName{ "FreePictureSplitter_" + QLocale(locale).name() };
        if (translator.load(":/i18n/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    fpsMainWindow w;
    w.show();

    int ret{ a.exec() };

    // Save configuration changes to file
    jsonCfgStr = QString::fromStdString(rfl::json::write(appConfig));
    ts.flush();
    ts << jsonCfgStr;
    cfgFile.close();

    return ret;
}
