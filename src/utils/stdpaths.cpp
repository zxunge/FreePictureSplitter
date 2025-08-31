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
