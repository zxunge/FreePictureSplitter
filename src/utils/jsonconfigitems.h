// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef JSONCONFIGITEMS_H
#define JSONCONFIGITEMS_H

#include <string>

namespace Util {

struct Config
{
    // We intrinsically use camelCase, thus reducing the header's size.
    struct
    {
        std::string name;
        std::string fullVersion;
        int majorVersion;
        int minorVersion;
        int microVersion;
        std::string style;
    } app;

    struct
    {
        std::string lastOpenedDir;
        std::string lastSavedToDir;
    } dialog;

    struct
    {

    } options;
};

} // namespace Util

#endif // JSONCONFIGITEMS_H
