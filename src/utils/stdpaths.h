// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef STDPATHS_H
#define STDPATHS_H

#include <QString>

namespace Util {

inline namespace StdPaths {

/*!
 * \brief getSkinsDir
 * \return The absolute path of the skins directory.
 */
QString skinsDir();

/*!
 * \brief getTranslationsDir
 * \return The absolute path of the translations directory.
 */
QString translationsDir();

/*!
 * \brief getDataDir
 * \return The absolute path of the data directory.
 * \details Data directory contains configuration files.
 */
QString dataDir();

} // namespace StdPaths

} // namespace Util

#endif // STDPATHS_H
