/*
 * This file is a part of FreePictureSplitter, a fast and easy-to-use picture splitter.
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
