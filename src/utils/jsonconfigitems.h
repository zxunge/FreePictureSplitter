// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
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
                         .style = "default" },
                .options = { .outputOpt = { .savingTo = SavingTo::inPlace,
                                            .subDir = false,
                                            .outFormat = "jpg",
                                            .jpgQuality = 80,
                                            .scalingFactor = 100.0 },
                             .gridOpt = { .enabled = false, .lineSize = 3, .colorRgb = "#ffffff" },
                             .nameOpt = { .prefMode = Prefix::same,
                                          .prefix = "splited",
                                          .rcContained = false },
                             .batchOpt = { .savingTo = SavingTo::same, .subDir = true } } };
}

} // namespace Util

#endif // JSONCONFIGITEMS_H
