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

#include "colors.h"

#include <cmath>

namespace Util {

QColor getDominantColorHSVWeighted(const QPixmap &pixmap)
{
    // Reduce the pixmap's size
    const QImage scaled{
        pixmap.scaled(64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).toImage()
    };

    // Store weighted mean
    double totalWeight{};
    double sumH{}, sumS{}, sumV{};

    for (int y{}; y < scaled.height(); ++y) {
        for (int x{}; x < scaled.width(); ++x) {
            QColor color{ scaled.pixelColor(x, y) };
            if (color.alpha() < 128)
                continue;

            float h{ color.hueF() };
            float s{ color.saturationF() };
            float v{ color.valueF() };

            if (h < 0)
                continue; // Skip invalid hue

            // Calculate weight (saturation + brightness enhancement of the perceived emphasis)
            double weight{ (s + 0.5) * (v + 0.3) };

            // Handle hue loop
            if (h < 0.25)
                h += 1.0; // Move red area to continuous intervals

            sumH += h * weight;
            sumS += s * weight;
            sumV += v * weight;
            totalWeight += weight;
        }
    }

    if (totalWeight <= 0)
        return QColor(128, 128, 128);

    // Calculate weighted mean
    float avgH{ fmodf(sumH / totalWeight, 1.0f) };
    float avgS{ qBound(0.0f, static_cast<float>(sumS / totalWeight), 1.0f) };
    float avgV{ qBound(0.0f, static_cast<float>(sumV / totalWeight), 1.0f) };

    return QColor::fromHsvF(avgH, avgS, avgV);
}

QColor getContrastColor(const QColor &color)
{
    // Convert to HSV and obtain the brightness component
    QColor hsv{ color.toHsv() };
    float value{ hsv.valueF() };

    // Use brightness threshold (adjustable)
    float threshold{ 0.6f }; // Use dark background if higher

    // Take saturation into account,
    // high saturation -> enhanced contrast color
    float saturation{ hsv.saturationF() };

    if (saturation > 0.7) {
        threshold -= 0.15f; // high saturation -> lower threshold
    }

    return (value > threshold) ? QColor(182, 182, 182) : QColor(240, 240, 240);
}

} // namespace Util
