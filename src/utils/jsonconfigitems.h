// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef JSONCONFIGITEMS_H
#define JSONCONFIGITEMS_H

#include <string>
#include <rfl/json.hpp>

namespace Util {

struct Config
{
    struct
    {
        std::string name;
        rfl::Rename<"fullVersion", std::string> full_version;
        rfl::Rename<"majorVersion", int> major_version;
        rfl::Rename<"minorVersion", int> minor_version;
        rfl::Rename<"microVersion", int> micro_version;
        std::string style;
    } app;

    struct
    {
        rfl::Rename<"lastOpenedDir", std::string> last_opened_dir;
    } dialog;

    struct
    {

    } options;
};

} // namespace Util

#endif // JSONCONFIGITEMS_H
