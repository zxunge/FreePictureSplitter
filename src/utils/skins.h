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
// clang-format Language: Cpp

#ifndef SKINS_H
#define SKINS_H

#include <QString>
#include <QStringList>

class QApplication;

namespace Util {
/********** Skins Helper **********/
// Get available skins names under DATADIR/skins directory
QStringList getAvailableSkins();

// Set application's stylesheet to the corresponding file with name `skinName'
bool setAppSkin(QApplication *app, const QString &skinName);
/**********************************/

} // namespace Util

#endif // SKINS_H
