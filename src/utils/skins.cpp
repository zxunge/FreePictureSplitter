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
#include <QStyleFactory>
#include <QMetaObject>

using namespace Qt::Literals::StringLiterals;

namespace Util {

QStringList getAvailableSkins()
{
    QStringList list;
    QDir skinDir(Util::getSkinsDir());
    for (const auto skin : skinDir.entryList(QStringList{ u"*.qss"_s }, QDir::Files)) {
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

    if (skin == u"default"_s) {
        // The default style includes a fusion style
        app->setStyle(QStyleFactory::create(u"fusion"_s));
        styleFile.setFileName(Util::getSkinsDir() + u"/default.qss"_s);
    } else
        styleFile.setFileName(Util::getSkinsDir() + '/' + skin + u".qss"_s);

    styleFile.open(QFile::ReadOnly);
    if (styleFile.isOpen()) {
        if (skin == u"lightblue"_s) // "lightblue" requires changing the palette
            app->setPalette(QPalette(u"#eaf7ff"_s));
        else
            app->setPalette(QPalette());
        app->setStyleSheet(QLatin1String(styleFile.readAll()));
        styleFile.close();
    } else {
        QMessageBox::warning(nullptr, fpsAppName, QObject::tr("Error loading skin."),
                             QMessageBox::Close);
        QMetaObject::invokeMethod(app, &QCoreApplication::quit, Qt::QueuedConnection);
    }
}

} // namespace Util
