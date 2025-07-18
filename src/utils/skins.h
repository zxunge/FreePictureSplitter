// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef SKINS_H
#define SKINS_H

#include <QString>
#include <QStringList>

class QApplication;

namespace Util {
/********** Skins Helper **********/
// Get available .qss files under DATADIR/skins directory
QStringList getAvailableSkins();

// Set application's stylesheet to skin.qss
void setAppSkin(QApplication *app, const QString &skin);
/**********************************/

} // namespace Util

#endif // SKINS_H
