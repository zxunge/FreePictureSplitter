// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSIMAGEHANDLER_H
#define FPSIMAGEHANDLER_H

#include <QString>
#include <QRect>
#include <QImage>
#include <QVector>
#include <QObject>

class fpsGraphicsView;

typedef QVector<QVector<QRect>> RectList;

/*
 * fpsImageHandler: Handles all the stuff for all-purpose image splitting and file generation.
 * The general workflow is:
 *  1. Users specify their wants, and we use `getSubRects' or `linesToRects' to capture them.
 *  2. Display our result using `rectsToLines'.
 *  3. Obtain all the sub-images using `split'.
 *  4. Obtain all the names of the files to save using `getOutputList'.
 *  5. Save files.
 */
class fpsImageHandler : public QObject
{
    Q_OBJECT
public:
    enum SplitMode { Size, Average };

    enum SplitSequence {
        Left = 0x0000000f,
        Right = 0x000000f0,
        Vert [[deprecated]] = 0x00000f00,
        Hori [[deprecated]] = 0x0000f000
    };
    Q_ENUM(SplitMode)
    Q_ENUM(SplitSequence)

public:
    // It's important to know that both our `SizeRect rect' and `QStringList list' have an
    // 'accessing' sequence of TopLeft -> BottomRight.
    static RectList getSubRects(int width, int height, int rowsOrHeight, int colsOrWidth,
                                SplitMode mode = Average, int32_t seq = Left);

    static bool split(const QImage &img, QVector<QImage> &output, const RectList &rects);

    static QStringList getOutputList(const QString &fileName, int rows, int cols);

    static void rectsToLines(const RectList &rects, fpsGraphicsView *parent);
    static RectList linesToRects(fpsGraphicsView *parent);

signals:
    void proceed(int progress);
};

#endif // FPSIMAGEHANDLER_H
