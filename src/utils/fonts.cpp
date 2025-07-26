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

using namespace Qt::Literals::StringLiterals;

namespace Util {

void setAppFont(QLocale locale, QGuiApplication *app)
{
    QFont font{ app->font() };

#if defined(Q_OS_WIN)
    switch (locale.language()) {
    case QLocale::Afrikaans:
    case QLocale::Albanian:
    case QLocale::Arabic:
    case QLocale::Armenian:
    case QLocale::English:
        font.setFamily(u"Segoe UI"_s);
        break;

    case QLocale::Amharic:
        font.setFamily(u"Ebrima"_s);
        break;

    case QLocale::Assamese:
        font.setFamily(u"Nirmala UI"_s);
        break;

    case QLocale::Chinese:
    case QLocale::Turkish:
    case QLocale::Greek:
        font.setFamily(u"Microsoft YaHei UI"_s);
        break;

    default:
        break;
    }
#else
    // Unix stuff
#endif

    font.setPointSize(9);
    app->setFont(font);
}

} // namespace Util
