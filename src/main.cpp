//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPLv3 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPLv3


#include "fpsmainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "qjsonconfig.h"

QJSONConfig cfg("conf.json");

int main(int argc, char *argv[])
{
    qputenv("QT_WIN_DEBUG_CONSOLE", "attach");
    qputenv("QSG_INFO", "1");

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
    QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    QApplication a(argc, argv);

    /* QCoreApplication::setApplicationName("FreePictureSplitter");
    QCoreApplication::setOrganizationName("zxunge (Grit Clef)"); */

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "FreePictureSplitter_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // First time?
    /* if (cfg.empty())
    {
        cfg.setValue("app/name", "FreePictureSplitter");
        cfg.setValue("app/author", "zxunge (Grit Clef)");
        cfg.setValue("style/isDark", true);
    } */

    fpsMainWindow w;
    w.show();

    int ret = a.exec();
    cfg.sync();
    return ret;
}
