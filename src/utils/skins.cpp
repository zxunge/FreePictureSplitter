// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "skins.h"
#include "stdpaths.h"

#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QDir>

#include <oclero/qlementine.hpp>

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
    QString skin(skinName.toLower());
    oclero::qlementine::QlementineStyle *style{ new oclero::qlementine::QlementineStyle(app) };
    if (skin != "default")
        style->setThemeJsonPath(Util::getSkinsDir() + '/' + skin + ".json");
    QApplication::setStyle(style);
}

} // namespace Util
