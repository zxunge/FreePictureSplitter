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
    for (const auto skin : skinDir.entryList(QStringList{ u"*.skin"_s }, QDir::Files)) {
        // Obtain the skin's name
        QFile file(skin);
        if (file.open(QFile::ReadOnly)) {
            QTextStream in(&file);
            const QString name{ in.readLine() };
            const QString placeholder{ in.readLine() };
            // A valid skin file
            if (!name.isEmpty() && placeholder == u"--@@##BEGIN##@@--"_s)
                list.push_back(name);
        }
    }
    return list;
}

bool setAppSkin(QApplication *app, const QString &skinName)
{
    QString realFn;
    QFile styleFile;

    if (skinName == u"Flat Light"_s) {
        // The default style includes a Win11Phantom style
        QWin11PhantomStyle *phStyle{ new QWin11PhantomStyle() };
        app->setStyle(phStyle);
        styleFile.setFileName(Util::getSkinsDir() + u"/default.skin"_s);
    } else {
        // Search for a skin file
        QDir skinDir(Util::getSkinsDir());

        for (const auto fileName : skinDir.entryList(QStringList{ u"*.skin"_s }, QDir::Files)) {
            QFile file(fileName);
            if (file.open(QFile::ReadOnly)) {
                QTextStream in(&file);
                const QString name{ in.readLine() };
                const QString placeholder{ in.readLine() };
                if (name == skinName && placeholder == u"--@@##BEGIN##@@--"_s) {
                    realFn = fileName;
                    break;
                }
            }
        }
        if (realFn.isEmpty())
            return false;

        // For other skins, we use `fusion`
        app->setStyle(QStyleFactory::create(u"fusion"_s));
        styleFile.setFileName(realFn);
    }

    if (styleFile.open(QFile::ReadOnly)) {
        if (skinName == u"Light Blue"_s) // "Light Blue" requires changing the palette
            app->setPalette(QPalette(u"#eaf7ff"_s));
        else
            app->setPalette(QPalette());

        QTextStream in(&styleFile);
        // The stylesheets cannot be applied directly,
        // we need to do some path-conversion.
        QString ss{ in.readAll().split(u"--@@##BEGIN##@@--"_s, Qt::SkipEmptyParts)[1] };
        ss.replace(u"@SKINS_DIR@"_s, Util::getSkinsDir());
        app->setStyleSheet(ss);
        styleFile.close();
    } else
        return false;
    return true;
}

} // namespace Util
