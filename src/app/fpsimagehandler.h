// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSIMAGEHANDLER_H
#define FPSIMAGEHANDLER_H

#include <QImage>
#include <QImageReader>
#include <QColor>
#include <QtCore/qstring.h>
#include <QtCore/qrect.h>
#include <QtCore/qvector.h>

class fpsGraphicsView;
class QPixmap;

typedef QVector<QVector<QRect>> RectList;

/*
 * fpsImageHandler: Handles all the stuff for all-purpose image splitting and file generation
 * (Single picture). The general workflow is:
 *  1. Users specify their wants, and we use `getSubRects' or `linesToRects' to capture them.
 *  2. Display our result using `rectsToLines'.
 *  3. Obtain all the sub-images using `split'.
 *  4. Obtain all the names of the files to save using `getOutputList'.
 *  5. [Optional] Draw a grid figure for the picture using `darwGridLines' with related arguments.
 *  6. Save files.
 */
class fpsImageHandler
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

#endif // FPSIMAGEHANDLER_H
