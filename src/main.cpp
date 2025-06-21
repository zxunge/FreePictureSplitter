// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsmainwindow.h"

#include <QLocale>
#include <QTranslator>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
            Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif

    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));
    QFile styleFile(QStringLiteral(":/skins/skins/default.qss"));
    styleFile.open(QFile::ReadOnly);
    if (styleFile.isOpen()) {
        a.setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    } else {
        QMessageBox::warning(nullptr, QStringLiteral("FreePictureSplitter"),
                             QStringLiteral("Error loading skin."), QMessageBox::Close);
        QApplication::exit(1);
    }

    QCoreApplication::setApplicationName("FreePictureSplitter");
    QCoreApplication::setOrganizationName("zxunge (Grit Clef)");

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

    int ret = a.exec();
    return ret;
}
