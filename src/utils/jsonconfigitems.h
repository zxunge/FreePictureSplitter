// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef JSONCONFIGITEMS_H
#define JSONCONFIGITEMS_H

#include "app_version.h"
#include "utils/thememanager.h"

#include <string>
#include <vector>

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
        std::string skin;
        std::string lang;
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

/*!
 * \brief setDefConf
 * \details Set the default configuration to \p s.
 */
inline void setDefConf(Config &s)
{
    s = Config{ .app = { .name = App::Constants::APP_NAME,
                         .fullVersion = App::Constants::APP_VERSION_STR,
                         .majorVersion = APP_VERSION_MAJOR,
                         .minorVersion = APP_VERSION_MINOR,
                         .microVersion = APP_VERSION_PATCH,
                         .skin = "Flat Light" },
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
Q_DECLARE_METATYPE(Util::Prefix)
Q_DECLARE_METATYPE(Util::SavingTo)
inline Util::Config g_appConfig; // Global configuration holder

#endif // JSONCONFIGITEMS_H
