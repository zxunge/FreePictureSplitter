//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsimagehandler.h"
#include <QRect>
#include <QFileInfo>
#include <QDir>
#include <QImage>


/* static */ QStringList fpsImageHandler::getOutputList(const QString &fileName, int rows, int cols)
{
    QStringList outputList;
    QFileInfo fi(fileName);
    auto splitCount { QString::asprintf("%d", rows * cols).size() };
    QString baseName { fi.baseName() }, suffix { fi.suffix() };

    for (int i {1}; i != rows * cols + 1; ++i)
        outputList << baseName + QStringLiteral("_") + QString::asprintf("%0*d", splitCount, i) + "." + suffix;
    return outputList;
}

/* static */ QVector<QVector<QRect>>
fpsImageHandler::getSubRects(int width,
                             int height,
                             int rowsOrHeight,
                             int colsOrWidth,
                             SplitMode mode,
                             SplitSequence seq)
{
    int rows, cols,                               // For rows and cols when splitting by pixel.
        s1rWidth, s1rHeight,                      // s1r: Step 1 Rectangles'
        s2rWidth, s2rHeight;                      // s2r: Step 2 Rectangles'

    // TODO@25/05/04 Add algorithm for different splitting sequences.

    //------------Split averagely-----------
    if (mode == SplitMode::Average)
    {
        s1rWidth  = width / colsOrWidth;
        s1rHeight = height / rowsOrHeight;
        s2rWidth  = width - (colsOrWidth - 1) * s1rWidth;
        s2rHeight = height - (rowsOrHeight - 1) * s1rHeight;
        rows      = rowsOrHeight;
        cols      = colsOrWidth;
    }
    // ------------Split by sizes-----------
    else
    {
        s1rHeight = rowsOrHeight;
        s1rWidth  = colsOrWidth;
        s2rWidth  = width - (colsOrWidth - 1) * s1rWidth;
        s2rHeight = height - (rowsOrHeight - 1) * s1rHeight;
        rows      = height / rowsOrHeight + 1;
        cols      = width / colsOrWidth + 1;
    }

    QVector<QVector<QRect>> subRects(rows, QVector<QRect>(cols));

    if ((!s1rHeight) || (!s1rWidth))
        return QVector<QVector<QRect>>(0);

    // Step 1：Get (rows - 1)(cols - 1) parts splited.
    for (int i {}; i < rows - 1; ++i)
        for (int j {}; j < cols - 1; ++j)
            subRects[i][j] = QRect(s1rWidth * j, s1rHeight * i, s1rWidth, s1rHeight);
    // Step 2：Get the last column * the last row splited.
    // In this case, we get the last row first (without the right-bottom part).
    for (int j {}; j < cols - 1; ++j)
        subRects[rows - 1][j] = QRect(s1rWidth * j, s1rHeight * (rows - 1), s1rWidth, s2rHeight);
    // Then get the last column splited (without the right-bottom part).
    for (int i {}; i < rows - 1; ++i)
        subRects[i][cols - 1] = QRect(s1rWidth * (cols - 1), s1rHeight * i, s2rWidth, s1rHeight);
    // Get the right-bottom part splited at last.
    subRects[rows - 1][cols - 1] = QRect(s1rWidth * (cols - 1), s1rHeight * (rows - 1), s2rWidth, s2rHeight);
    // OK! Task is finished. Return......
    return subRects;
}

[[ nodiscard ]] /* static */ bool fpsImageHandler::split(const QImage &img,
                                                         QVector<QImage> &output,
                                                         const QVector<QVector<QRect>> &rects)
{
    if (rects.isEmpty())
        return false;

    auto cRows { rects.size() }, cCols { rects[0].size() };

    for (int j {}; j < cRows; ++j)
        for (int k {}; k < cCols; ++k)
            output.push_back(img.copy(rects[j][k]));

    return true;
}
