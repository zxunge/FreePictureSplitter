// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef STDPATHS_H
#define STDPATHS_H

#include <QString>

namespace Util {

/*********** Standard Paths Helper ***********/
// Get the skins directory
QString getSkinsDir();

// Get the translations directory
QString getTranslationsDir();

// Get the application's data directory
QString getDataDir();
/*********************************************/

} // namespace Util

#endif // STDPATHS_H
