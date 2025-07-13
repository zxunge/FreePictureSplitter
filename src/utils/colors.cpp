// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            float h{ color.hueF() };
            float s{ color.saturationF() };
            float v{ color.valueF() };
#else
            qreal h{ color.hueF() };
            qreal s{ color.saturationF() };
            qreal v{ color.valueF() };
#endif

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
    double avgH{ fmod(sumH / totalWeight, 1.0) };
    float avgS{ qBound(0.0f, static_cast<float>(sumS / totalWeight), 1.0f) };
    float avgV{ qBound(0.0f, static_cast<float>(sumV / totalWeight), 1.0f) };

    // Adjust red area
    if (avgH > 0.75)
        avgH -= 1.0;

    return QColor::fromHsvF(avgH, avgS, avgV);
}

QColor getContrastColor(const QColor &color)
{
    // Convert to HSV and obtain the brightness component
    QColor hsv{ color.toHsv() };

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    float value{ hsv.valueF() };
#else
    qreal value{ hsv.valueF() };
#endif

    // Use brightness threshold (adjustable)
    float threshold{ 0.6f }; // Use dark background if higher

    // Take saturation into account,
    // high saturation -> enhanced contrast color
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    float saturation{ hsv.saturationF() };
#else
    qreal saturation{ hsv.saturationF() };
#endif

    if (saturation > 0.7) {
        threshold -= 0.15f; // high saturation -> lower threshold
    }

    return (value > threshold) ? QColor(182, 182, 192) : QColor(240, 240, 245);
}

} // namespace Util
