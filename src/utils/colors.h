// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef COLORS_H
#define COLORS_H

#include <QColor>
#include <QPixmap>

// Extract a primary hue using HSV color space
QColor getDominantColorHSVWeighted(const QPixmap &pixmap);

// Calculate the contrast color through perceived brightness
QColor getContrastColor(const QColor &color);

#endif // COLORS_H
