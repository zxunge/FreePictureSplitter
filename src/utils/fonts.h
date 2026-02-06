// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef FONTS_H
#define FONTS_H

#include <QLocale>
#include <QGuiApplication>

namespace Util {

inline namespace Font {

/*!
 * \brief setAppFont
 * \details Automatically set the application's font using a locale.
 */
inline void setAppFont(QLocale locale, QGuiApplication *app)
{
    Q_UNUSED(locale);
    QFont font{ app->font() };
    font.setPointSize(9);
    app->setFont(font);
}

} // namespace Font

} // namespace Util

#endif // FONTS_H
