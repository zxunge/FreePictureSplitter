// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsimagehandler.h"
#include "fpsgraphicsview.h"
#include "fpsfloatingline.h"

#include <QImage>
#include <QRect>

#include <algorithm>

/* static */ QStringList fpsImageHandler::getOutputList(const QString &prefix,
                                                        const QString &suffix, int rows, int cols,
                                                        bool rcContained)
{
    QStringList outputList;
    auto splitCount{ QString::asprintf("%d", rows * cols).size() };

    for (int i{ 1 }; i != rows + 1; ++i)
        for (int j{ 1 }; j != cols + 1; ++j)
            outputList << prefix + QStringLiteral("_")
                            + QString::asprintf("%0*d", static_cast<int>(splitCount),
                                                (i - 1) * cols + j)
                            + (rcContained ? QString::asprintf("_%dx%d.", i, j)
                                           : QStringLiteral("."))
                            + suffix;
    return outputList;
}

/* static */ RectList fpsImageHandler::getSubRects(int width, int height, int rowsOrHeight,
                                                   int colsOrWidth, SplitMode mode, int32_t seq)
{
    // TODO@25/05/10 Improve algorithm.
    // There're 2 steps we should follow basically:
    // First, write rows info to `rects'
    // Then, cols info.
    if (width * height == 0 || (mode == Size && (rowsOrHeight > height || colsOrWidth > width))
        || (mode == Average && rowsOrHeight * colsOrWidth == 0))
        return RectList();

    int basicRowHeight, basicColWidth, legacyRowHeight, legacyColWidth;
    int rows, cols;
    RectList rects;
    // We need info about how many rows and columns when splitting by sizes ourself.
    switch (mode) {
    case Size:
        basicColWidth = colsOrWidth;
        basicRowHeight = rowsOrHeight;
        rows = (height % rowsOrHeight == 0) ? height / rowsOrHeight : height / rowsOrHeight + 1;
        cols = (width % colsOrWidth == 0) ? width / colsOrWidth : width / colsOrWidth + 1;

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
    if ((seq & Left) == Left) {
        // ----- Rows: Up -> Down -----
        for (int i{}; i != rows - 1; ++i)
            for (int j{}; j != cols; ++j) {
                rects[i][j].setTop(i * basicRowHeight);
                rects[i][j].setBottom((i + 1) * basicRowHeight);
            }
        // Legacy;
        for (int j{}; j != cols; ++j) {
            rects[rows - 1][j].setTop((rows - 1) * basicRowHeight);
            rects[rows - 1][j].setHeight(legacyRowHeight);
        }
        // ----- Columns: L -> R -----
        for (int i{}; i != rows; ++i)
            for (int j{}; j != cols - 1; ++j) {
                rects[i][j].setLeft(j * basicColWidth);
                rects[i][j].setRight((j + 1) * basicColWidth);
            }
        // Legacy;
        for (int i{}; i != rows; ++i) {
            rects[i][cols - 1].setLeft((cols - 1) * basicColWidth);
            rects[i][cols - 1].setWidth(legacyColWidth);
        }
    } else if ((seq & Right) == Right) {
        // ----- Rows: Up -> Down -----
        for (int i{}; i != rows - 1; ++i)
            for (int j{}; j != cols; ++j) {
                rects[i][j].setTop(i * basicRowHeight);
                rects[i][j].setBottom((i + 1) * basicRowHeight);
            }
        // Legacy;
        for (int j{}; j != cols; ++j) {
            rects[rows - 1][j].setTop((rows - 1) * basicRowHeight);
            rects[rows - 1][j].setHeight(legacyRowHeight);
        }
        // ----- Columns: R -> L -----
        for (int i{}; i != rows; ++i)
            for (int j{ cols - 1 }; j != 0; --j) {
                rects[i][j].setLeft(j * basicColWidth);
                rects[i][j].setRight((j + 1) * basicColWidth);
            }
        // Legacy;
        for (int i{}; i != rows; ++i) {
            rects[i][0].setLeft(0);
            rects[i][0].setWidth(legacyColWidth);
        }
    } else
        return RectList();

    return rects;
}

[[nodiscard]] /* static */ bool
fpsImageHandler::split(QImageReader &imgReader, QVector<QImage> &output, const RectList &rects)
{
    if (rects.isEmpty())
        return false;

    auto cRows{ rects.size() }, cCols{ rects[0].size() };
    // Avoid has been read
    imgReader.setFileName(imgReader.fileName());
    const QImage img{ imgReader.read() };

    if (img.isNull())
        return false;

    output.clear();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    for (qsizetype j{}; j != cRows; ++j)
        for (qsizetype k{}; k != cCols; ++k)
#else
    for (int j{}; j != cRows; ++j)
        for (int k{}; k != cCols; ++k)
#endif
            output.push_back(img.copy(rects[j][k]));

    return true;
}

/* static */ void fpsImageHandler::rectsToLines(const RectList &rects, fpsGraphicsView *parent)
{
    if (0 == rects.size() || 0 == rects[0].size())
        return;

    fpsFloatingLine *line{};
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    for (qsizetype i{}; i != rects.size() - 1; ++i) {
#else
    for (int i{}; i != rects.size() - 1; ++i) {
#endif
        line = new fpsFloatingLine(parent);
        line->setScenePos(rects[i][0].bottom());
        line->show();
        parent->addFloatingLine(line);
        line = nullptr;
    }
    for (int i{}; i != rects[0].size() - 1; ++i) {
        line = new fpsFloatingLine(parent, Qt::Vertical);
        line->setScenePos(rects[0][i].right());
        line->show();
        parent->addFloatingLine(line);
        line = nullptr;
    }
}

/* static */ RectList fpsImageHandler::linesToRects(fpsGraphicsView *parent)
{
    QVector<int> vx, vy;

    for (auto l : parent->getFloatingLines())
        if (l->getOrientation() == Qt::Horizontal)
            vy.push_back(l->getScenePos());
        else
            vx.push_back(l->getScenePos());
    std::sort(vx.begin(), vx.end());
    std::sort(vy.begin(), vy.end());

    RectList rects;
    rects.resize(vy.size() + 1);
    for (auto &r : rects)
        r.resize(vx.size() + 1);

    // Axis X
    for (int i{}; i <= vy.size(); ++i)
        rects[i][0].setLeft(0);
    for (int i{}; i <= vy.size(); ++i)
        rects[i][vx.size()].setRight(parent->scene()->width() - 1);
    for (int i{}; i <= vy.size(); ++i)
        for (int j{}; j != vx.size(); ++j) {
            rects[i][j].setRight(vx[j]);
            rects[i][j + 1].setLeft(vx[j]);
        }

    // Axis Y
    for (int j{}; j <= vx.size(); ++j)
        rects[0][j].setTop(0);
    for (int j{}; j <= vx.size(); ++j)
        rects[vy.size()][j].setBottom(parent->scene()->height() - 1);
    for (int i{}; i != vy.size(); ++i)
        for (int j{}; j <= vx.size(); ++j) {
            rects[i][j].setBottom(vy[i]);
            rects[i + 1][j].setTop(vy[i]);
        }

    return rects;
}
