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

#ifndef JSONCONFIGITEMS_H
#define JSONCONFIGITEMS_H

#include "config.h"
#include <string>

namespace Util {

enum class SavingTo { same, inPlace, specified };
enum class Prefix { same, specified };

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
        int lastEnteredIndex;
    } dialog;

    struct
    {
        struct
        {
            SavingTo savingTo;
            bool subDir;
            std::string outPath;
            std::string outFormat;
            int jpgQuality;
            double scalingFactor;
        } outputOpt;

        struct
        {
            bool enabled;
            int lineSize;
            std::string colorRgb; // Formed as "#RRGGBB"
        } gridOpt;

        struct
        {
            Prefix prefMode;
            std::string prefix;
            bool rcContained;
        } nameOpt;

        struct
        {
            SavingTo savingTo;
            bool subDir;
            std::string outPath;
        } batchOpt;
    } options;
};

inline void setDefConf(Config &s)
{
    s = Config{ .app = { .name = fpsAppName,
                         .fullVersion = fpsVersionFull,
                         .majorVersion = fpsVersionMajor,
                         .minorVersion = fpsVersionMinor,
                         .microVersion = fpsVersionMicro,
                         .style = "Flat Light" },
                .dialog = { .lastEnteredIndex = 0 },
                .options = { .outputOpt = { .savingTo = SavingTo::inPlace,
                                            .subDir = false,
                                            .outFormat = "jpg",
                                            .jpgQuality = 80,
                                            .scalingFactor = 1.0 },
                             .gridOpt = { .enabled = false, .lineSize = 3, .colorRgb = "#ffffff" },
                             .nameOpt = { .prefMode = Prefix::same,
                                          .prefix = "splited",
                                          .rcContained = false },
                             .batchOpt = { .savingTo = SavingTo::same, .subDir = true } } };
}

} // namespace Util

#endif // JSONCONFIGITEMS_H
