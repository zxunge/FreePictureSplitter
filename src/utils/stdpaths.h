/*
 * This file is part of FreePictureSplitter, a fast and easy-to-use picture splitter.
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

#ifndef STDPATHS_H
#define STDPATHS_H

#include <QString>

namespace Util {

//---------- Standard Paths Helper ----------
/*!
 * \brief getSkinsDir
 * \return The absolute path of the skins directory.
 */
QString getSkinsDir();

/*!
 * \brief getTranslationsDir
 * \return The absolute path of the translations directory.
 */
QString getTranslationsDir();

/*!
 * \brief getDataDir
 * \return The absolute path of the data directory.
 * \details Data directory contains configuration files.
 */
QString getDataDir();

} // namespace Util

#endif // STDPATHS_H
