// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "colors.h"

namespace Util {

inline namespace Color {

using namespace Qt::Literals::StringLiterals;

QColor getDominantColor(const QPixmap &pixmap)
{
    // Reduce the pixmap's size
    const QImage scaled{
        pixmap.scaled(32, 32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).toImage()
    };

    // Average color
    qint64 r{}, g{}, b{}, count{};
    QColor dominant;

    for (int y{}; y < scaled.height(); ++y) {
        for (int x{}; x < scaled.width(); ++x) {
            QColor color{ scaled.pixelColor(x, y) };
            if (color.alpha() > 128) { // Ignore transparent pixels
                r += color.red();
                g += color.green();
                b += color.blue();
                ++count;
            }
        }
    }

    if (count == 0)
        dominant = Qt::gray; // Default
    dominant.setRgb(r / count, g / count, b / count);
    return dominant;
}

QColor getContrastColor(const QColor &color)
{
    // See https://www.w3.org/TR/WCAG20/#relativeluminancedef
    double luminance{ 0.2126 * color.redF() + 0.7152 * color.greenF() + 0.0722 * color.blueF() };
    return (luminance > 0.5) ? QColor::fromString(u"#c0c0c0"_s) : QColor::fromString(u"#ffffff"_s);
}

} // namespace Color

} // namespace Util

