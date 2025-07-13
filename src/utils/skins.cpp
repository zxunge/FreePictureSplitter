// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "skins.h"

#include <QFile>
#include <QMessageBox>
#include <QMetaObject>
#include <QCoreApplication>
#include <QApplication>
#include <QDir>
#include <QStyleFactory>

namespace Util {

QStringList getAvailableSkins()
{
    QStringList list;
    QDir skinDir(QCoreApplication::applicationDirPath() + "/skins");
    for (const auto skin : skinDir.entryList(QStringList{ "*.qss" }, QDir::Files))
        list.push_back(QFileInfo(skin).baseName());
    return list;
}

void setAppSkin(QApplication *app, const QString &skin)
{
    QFile styleFile;
    if (skin == "default") {
        // The default style includes a fusion style
        app->setStyle(QStyleFactory::create("fusion"));
        styleFile.setFileName(QCoreApplication::applicationDirPath() + "/skins/default.qss");
    } else
        styleFile.setFileName(QCoreApplication::applicationDirPath() + "/skins/" + skin + ".qss");

    styleFile.open(QFile::ReadOnly);
    if (styleFile.isOpen()) {
        if (skin == "lightblue") // "lightblue" requires changing the palette
            app->setPalette(QPalette("#eaf7ff"));
        app->setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    } else {
        QMessageBox::warning(nullptr, QStringLiteral("FreePictureSplitter"),
                             QObject::tr("Error loading skin."), QMessageBox::Close);
        QMetaObject::invokeMethod(app, &QCoreApplication::quit, Qt::QueuedConnection);
    }
}

} // namespace Util
