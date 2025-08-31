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

#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QImage>
#include <QImageReader>
#include <QColor>
#include <QtCore/qstring.h>
#include <QtCore/qrect.h>
#include <QtCore/qvector.h>

class fpsGraphicsView;
class QPixmap;

namespace Util {

typedef QVector<QVector<QRect>> RectList;

/*!
 * \brief The ImageHandler class: Handles all the stuff for all-purpose image splitting and file
 * generation (Single picture).
 * \class ImageHandler
 * \include imagehandler.h
 * \details The general workflow is:
 *  1. Users specify their wants, and we use \code getSubRects \endcode or \code linesToRects
 * \endcode to capture them.
 *  2. Display our result using \code rectsToLines \endcode.
 *  3. Obtain all the sub-images using \code split \endcode.
 *  4. Obtain all the names of the files to save using \code getOutputList \endcode.
 *  5. [Optional] Draw a grid figure for the picture using \code darwGridLines \endcode with related
 * arguments.
 *  6. Save files.
 */
class ImageHandler
{
public:
    enum class SplitMode { Size, Average };

    enum SplitSequence {
        Left = 0x0000000f,
        Right = 0x000000f0,
        Vert [[deprecated]] = 0x00000f00,
        Hori [[deprecated]] = 0x0000f000
    };

public:
    // It's important to know that both our `SizeRect rect' and `QStringList list' have an
    // 'accessing' sequence of TopLeft -> BottomRight.
    static RectList getSubRects(int width, int height, int rowsOrHeight, int colsOrWidth,
                                SplitMode mode = SplitMode::Average,
                                SplitSequence seq = SplitSequence::Left);

    static bool split(QImageReader &imgReader, QVector<QImage> &output, const RectList &rects);

    static QStringList getOutputList(const QString &prefix, const QString &suffix, int rows,
                                     int cols, bool rcContained = false, bool grid = false);

    static void rectsToLines(const RectList &rects, fpsGraphicsView *parent);
    static RectList linesToRects(fpsGraphicsView *parent);

    static void drawGridLines(QPixmap *pixmap, const RectList &rects, const QColor &color,
                              int size);
};

} // namespace Util

#endif // IMAGEHANDLER_H
