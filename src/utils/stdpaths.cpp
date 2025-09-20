// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

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
    return QDir::cleanPath(dir.absolutePath());
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
    return QDir::cleanPath(dir.absolutePath());
}

QString getDataDir()
{
#if defined(Q_OS_WIN) && !defined(WIN32_GNU_DEPLOY)
    QDir dir(QLibraryInfo::path(QLibraryInfo::BinariesPath));
#elif defined(Q_OS_UNIX) || defined(WIN32_GNU_DEPLOY)
    QDir dir(QLibraryInfo::path(QLibraryInfo::DataPath));
    dir.cd(u"fps"_s);
#endif
    return QDir::cleanPath(dir.absolutePath());
}

} // namespace Util
