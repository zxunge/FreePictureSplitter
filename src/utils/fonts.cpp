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

#include "fonts.h"

#include <QGuiApplication>
#include <QFont>

namespace Util {

void setAppFont(QLocale locale, QGuiApplication *app)
{
    QFont font;
    switch (locale.language()) {
    case QLocale::English:
    case QLocale::Chinese:
    case QLocale::Turkish:
    case QLocale::Greek:
#if defined(Q_OS_WIN)
        font.setFamily("Microsoft YaHei UI");
#endif
        break;

    default:
        break;
    }
    font.setPixelSize(13);
    app->setFont(font);
}

} // namespace Util