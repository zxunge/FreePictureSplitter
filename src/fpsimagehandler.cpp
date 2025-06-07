//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) 2024 2025 zxunge
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
        outputList << baseName + QStringLiteral("_") + QString::asprintf("%0*d", static_cast<int>(splitCount), i) + "." + suffix;
    return outputList;
}

/* static */ QVector<QVector<QRect>>
fpsImageHandler::getSubRects(int width,
                             int height,
                             int rowsOrHeight,
                             int colsOrWidth,
                             SplitMode mode,
                             int32_t seq)
{
    // TODO@25/05/10 Improve algorithm.
    // There're 2 steps we should follow basically:
    // First, write rows info to `rects'
    // Then, cols info.
    if (width * height == 0 ||
        (mode == Size && (rowsOrHeight > height || colsOrWidth > width)) || 
        (mode == Average && rowsOrHeight * colsOrWidth == 0))
        return QVector<QVector<QRect>>();
        
    int basicRowHeight, basicColWidth, legacyRowHeight, legacyColWidth;
    int rows, cols;
    QVector<QVector<QRect>> rects;
    // We need info about how many rows and columns when splitting by sizes ourself.
    switch (mode)
    {
    case Size:
        basicColWidth = colsOrWidth;
        basicRowHeight = rowsOrHeight;
        rows = (height % rowsOrHeight == 0) ? height / rowsOrHeight
                                            : height / rowsOrHeight + 1;
        cols = (width % colsOrWidth == 0) ? width / colsOrWidth
                                          : width / colsOrWidth + 1;
        
        break;
    case Average:
        // TODO@25/05/04 Use more smooth splitting differences.
        basicRowHeight = height / rowsOrHeight;
        basicColWidth = width / colsOrWidth;
        rows = rowsOrHeight;
        cols = colsOrWidth;
        break;
    }
    legacyColWidth = width - basicColWidth * (cols - 1);
    legacyRowHeight = height - basicRowHeight * (rows - 1);

    // Initialize sizes
    rects.resize(rows);
    for (auto &i : rects)
        i.resize(cols);
    
    /* // ----- Rows: Down -> Up -----
        for (int i { rows - 1 }; i != 0; --i)
            for (int j {}; j != cols; ++j)
            {
                rects[i][j].setTop(i * basicRowHeight);
                rects[i][j].setBottom((i + 1) * basicRowHeight);
            }
        // Legacy;
        for (int j {}; j != cols; ++j)
        {
            rects[0][j].setTop(0);
            rects[0][j].setHeight(legacyRowHeight);
        }
    */
    // Some codes are duplicated.
    if ((seq & Left) == Left)
    {
        // ----- Rows: Up -> Down -----
        for (int i {}; i != rows - 1; ++i)
            for (int j {}; j != cols; ++j)
            {
                rects[i][j].setTop(i * basicRowHeight);
                rects[i][j].setBottom((i + 1) * basicRowHeight);
            }
        // Legacy;
        for (int j {}; j != cols; ++j)
        {
            rects[rows - 1][j].setTop((rows - 1) * basicRowHeight);
            rects[rows - 1][j].setHeight(legacyRowHeight);
        }
        // ----- Columns: L -> R -----
        for (int i {}; i != rows; ++i)
            for (int j {}; j != cols - 1; ++j)
            {
                rects[i][j].setLeft(j * basicColWidth);
                rects[i][j].setRight((j + 1) * basicColWidth);
            }
        // Legacy;
        for (int i {}; i != rows; ++i)
        {
            rects[i][cols - 1].setLeft((cols - 1) * basicColWidth);
            rects[i][cols - 1].setWidth(legacyColWidth);
        }
    }
    else if ((seq & Right) == Right)
    {
        // ----- Rows: Up -> Down -----
        for (int i {}; i != rows - 1; ++i)
            for (int j {}; j != cols; ++j)
            {
                rects[i][j].setTop(i * basicRowHeight);
                rects[i][j].setBottom((i + 1) * basicRowHeight);
            }
        // Legacy;
        for (int j {}; j != cols; ++j)
        {
            rects[rows - 1][j].setTop((rows - 1) * basicRowHeight);
            rects[rows - 1][j].setHeight(legacyRowHeight);
        }
        // ----- Columns: R -> L -----
        for (int i {}; i != rows; ++i)
            for (int j { cols - 1 }; j != 0; --j)
            {
                rects[i][j].setLeft(j * basicColWidth);
                rects[i][j].setRight((j + 1) * basicColWidth);
            }
        // Legacy;
        for (int i {}; i != rows; ++i)
        {
            rects[i][0].setLeft(0);
            rects[i][0].setWidth(legacyColWidth);
        }
    }
    else
        return QVector<QVector<QRect>>();

    return rects;
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
