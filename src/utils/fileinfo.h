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

#ifndef FILEINFO_H
#define FILEINFO_H

#include <QString>
#include <QFileInfo>

using namespace Qt::Literals::StringLiterals;

namespace Util {

QString getFileSizeString(const QString &fn)
{
    constexpr qint64 kilobyte{ 1024 };
    constexpr qint64 megabyte{ 1048576 };
    constexpr qint64 gigabyte{ 1073741824 };
    const qint64 size{ QFileInfo(fn).size() };

    if (size >= gigabyte)
        return QString::number(static_cast<double>(size) / gigabyte, 'f', 2) % u" GB"_s;
    else if (size >= megabyte)
        return QString::number(static_cast<double>(size) / megabyte, 'f', 2) % u" MB"_s;
    else if (size >= kilobyte)
        return QString::number(static_cast<double>(size) / kilobyte, 'f', 2) % u" KB"_s;
    else
        return QString::number(size) % u" Bytes"_s;
}

} // namespace Util

#endif // FILEINFO_H
