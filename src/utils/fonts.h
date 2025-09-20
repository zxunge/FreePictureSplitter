// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef FONTS_H
#define FONTS_H

#include <QLocale>

class QGuiApplication;

namespace Util {

//---------- Application Fonts Helper ----------
/*!
 * \brief setAppFont
 * \details Automatically set the application's font using a locale.
 */
void setAppFont(QLocale locale, QGuiApplication *app);

} // namespace Util

#endif // FONTS_H