// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "stdpaths.h"

#include <QDir>
#include <QLibraryInfo>
#include <QStringLiteral>

namespace Util {

QString getSkinsDir()
{
#if defined(Q_OS_WIN) && !defined(WIN32_GNU_DEPLOY)
#  if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDir dir(QLibraryInfo::path(QLibraryInfo::PrefixPath));
#  else
    QDir dir(QLibraryInfo::location(QLibraryInfo::PrefixPath));
#  endif
    dir.cd(QStringLiteral(u"skins"));
#elif defined(Q_OS_UNIX) || defined(WIN32_GNU_DEPLOY)
#  if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDir dir(QLibraryInfo::path(QLibraryInfo::DataPath));
#  else
    QDir dir(QLibraryInfo::location(QLibraryInfo::DataPath));
#  endif
    dir.cd(QStringLiteral(u"fps"));
    dir.cd(QStringLiteral(u"skins"));
#endif
    return dir.absolutePath();
}

QString getTranslationsDir()
{
#if defined(Q_OS_WIN) && !defined(WIN32_GNU_DEPLOY)
#  if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDir dir(QLibraryInfo::path(QLibraryInfo::PrefixPath));
#  else
    QDir dir(QLibraryInfo::location(QLibraryInfo::PrefixPath));
#  endif
    dir.cd(QStringLiteral(u"translations"));
#elif defined(Q_OS_UNIX) || defined(WIN32_GNU_DEPLOY)
#  if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDir dir(QLibraryInfo::path(QLibraryInfo::DataPath));
#  else
    QDir dir(QLibraryInfo::location(QLibraryInfo::DataPath));
#  endif
    dir.cd(QStringLiteral(u"fps"));
    dir.cd(QStringLiteral(u"translations"));
#endif
    return dir.absolutePath();
}

QString getDataDir()
{
#if defined(Q_OS_WIN) && !defined(WIN32_GNU_DEPLOY)
#  if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDir dir(QLibraryInfo::path(QLibraryInfo::BinariesPath));
#  else
    QDir dir(QLibraryInfo::location(QLibraryInfo::BinariesPath));
#  endif
#elif defined(Q_OS_UNIX) || defined(WIN32_GNU_DEPLOY)
#  if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QDir dir(QLibraryInfo::path(QLibraryInfo::DataPath));
#  else
    QDir dir(QLibraryInfo::location(QLibraryInfo::DataPath));
#  endif
    dir.cd(QStringLiteral(u"fps"));
#endif
    return dir.absolutePath();
}

} // namespace Util
