// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "skins.h"
#include "stdpaths.h"
#include "config.h"

#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QDir>

namespace Util {

QStringList getAvailableSkins()
{
    QStringList list;
    QDir skinDir(Util::getSkinsDir());
    for (const auto skin : skinDir.entryList(QStringList{ "*.json" }, QDir::Files)) {
        const QString baseName{ QFileInfo(skin).baseName() };
        // Capitalize the first letter
        list.push_back(baseName.left(1).toUpper()
                       + baseName.mid(1, baseName.length() - 1).toLower());
    }
    return list;
}

void setAppSkin(QApplication *app, const QString &skinName)
{
    QFile styleFile;
    // Normalize the skin name
    const QString skin{ skinName.toLower() };

    if (skin == "default") {
        // The default style includes a fusion style
        app->setStyle(QStyleFactory::create("fusion"));
        styleFile.setFileName(Util::getSkinsDir() + "/default.qss");
    } else
        styleFile.setFileName(Util::getSkinsDir() + '/' + skin + ".qss");

    styleFile.open(QFile::ReadOnly);
    if (styleFile.isOpen()) {
        if (skin == "lightblue") // "lightblue" requires changing the palette
            app->setPalette(QPalette("#eaf7ff"));
        else
            app->setPalette(QPalette());
        app->setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    } else {
        QMessageBox::warning(nullptr, fpsAppName,
                             QObject::tr("Error loading skin."), QMessageBox::Close);
        QMetaObject::invokeMethod(app, &QCoreApplication::quit, Qt::QueuedConnection);
    }
}

} // namespace Util
