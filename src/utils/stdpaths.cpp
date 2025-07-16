// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "stdpaths.h"

#include <QDir>
#include <QLibraryInfo>

using namespace Qt::Literals::StringLiterals;

namespace Util {

QString getSkinsDir()
{
#if defined(Q_OS_WIN) && !defined(WIN32_GNU_DEPLOY)
    QDir dir(QLibraryInfo::path(QLibraryInfo::PrefixPath));
    dir.cd(u"skins"_s);
#elif defined(Q_OS_UNIX) || defined(WIN32_GNU_DEPLOY)
    QDir dir(QLibraryInfo::path(QLibraryInfo::DataPath));
    dir.cd(u"fps"_s);
    dir.cd(u"skins"_s);
#endif
    return dir.absolutePath();
}

QString getTranslationsDir()
{
#if defined(Q_OS_WIN) && !defined(WIN32_GNU_DEPLOY)
    QDir dir(QLibraryInfo::path(QLibraryInfo::PrefixPath));
    dir.cd(u"translations"_s);
#elif defined(Q_OS_UNIX) || defined(WIN32_GNU_DEPLOY)
    QDir dir(QLibraryInfo::path(QLibraryInfo::DataPath));
    dir.cd(u"fps"_s);
    dir.cd(u"translations"_s);
#endif
    return dir.absolutePath();
}

} // namespace Util
