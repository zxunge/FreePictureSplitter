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
// Get the app's install prefix
QString getInstallPrefix();

// Get the app's DATADIR
QString getDataDir();

// Get the app's LIBDIR
QString getLibDir();

// Get the app's BINDIR
QString getBinDir();
/*********************************************/

} // namespace Util

#endif // STDPATHS_H
