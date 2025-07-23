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

#include "skins.h"
#include "stdpaths.h"
#include "config.h"

#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QMetaObject>
#include <QStyleFactory>

#include <qwin11phantomstyle.h> // For a modern look on the default skin

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
        // The default style includes a Win11Phantom style
        QWin11PhantomStyle *phStyle{ new QWin11PhantomStyle() };
        app->setStyle(phStyle);
        styleFile.setFileName(Util::getSkinsDir() + u"/default.qss"_s);
    } else {
        // For other skins, we use `fusion`
        app->setStyle(QStyleFactory::create(u"fusion"_s));
        styleFile.setFileName(Util::getSkinsDir() + '/' + skin + u".qss"_s);
    }

    styleFile.open(QFile::ReadOnly);
    if (styleFile.isOpen()) {
        if (skin == u"lightblue"_s) // "lightblue" requires changing the palette
            app->setPalette(QPalette(u"#eaf7ff"_s));
        else
            app->setPalette(QPalette());
        app->setStyleSheet(QString(styleFile.readAll()));
        styleFile.close();
    } else {
        QMessageBox::warning(nullptr, fpsAppName, QObject::tr("Error loading skin."),
                             QMessageBox::Close);
        QMetaObject::invokeMethod(app, &QCoreApplication::quit, Qt::QueuedConnection);
    }
}

} // namespace Util
