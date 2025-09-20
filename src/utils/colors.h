// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef COLORS_H
#define COLORS_H

#include <QColor>
#include <QPixmap>

namespace Util {

/*!
 * \brief getDominantColor
 * \details Extract a primary hue of the whole \p pixmap.
 */
QColor getDominantColor(const QPixmap &pixmap);

/*!
 * \brief getContrastColor
 * \details Calculate the contrast color through perceived brightness.
 */
QColor getContrastColor(const QColor &color);

} // namespace Util

#endif // COLORS_H
