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

#include "imagehandler.h"
#include "graphicsview.h"
#include "draggableline.h"
#include "rulerbar.h"

#include <QImage>
#include <QRect>
#include <QPixmap>
#include <QPainter>
#include <QPen>

#include <algorithm>
#include <cmath>

using namespace Qt::Literals::StringLiterals;

namespace Util {

/* static */ QStringList ImageHandler::getOutputList(const QString &prefix, const QString &suffix,
                                                     int rows, int cols, bool rcContained,
                                                     bool grid)
{
    QStringList outputList;
    auto splitCount{ QString::asprintf("%d", rows * cols).size() };

    for (int i{ 1 }; i != rows + 1; ++i)
        for (int j{ 1 }; j != cols + 1; ++j)
            outputList << prefix + u"_"_s
                            + QString::asprintf("%0*d", static_cast<int>(splitCount),
                                                (i - 1) * cols + j)
                            + (rcContained ? QString::asprintf("_%dx%d.", i, j) : u"."_s) + suffix;
    if (grid)
        outputList << prefix + u"_grid."_s + suffix;
    return outputList;
}

/* static */ RectList ImageHandler::getSubRects(int width, int height, int rowsOrHeight,
                                                int colsOrWidth, SplitMode mode, SplitSequence seq)
{
    // TODO@25/05/10 Improve algorithm.
    // There're 2 steps we should follow basically:
    // First, write rows info to `rects'
    // Then, cols info.
    if (width * height == 0
        || (mode == SplitMode::Size && (rowsOrHeight > height || colsOrWidth > width))
        || (mode == SplitMode::Average && rowsOrHeight * colsOrWidth == 0))
        return RectList();

    int basicRowHeight, basicColWidth, legacyRowHeight, legacyColWidth;
    int rows, cols;
    RectList rects;
    // We need info about how many rows and columns when splitting by sizes ourself.
    switch (mode) {
    case SplitMode::Size:
        basicColWidth = colsOrWidth;
        basicRowHeight = rowsOrHeight;
        rows = (height % rowsOrHeight == 0) ? height / rowsOrHeight : height / rowsOrHeight + 1;
        cols = (width % colsOrWidth == 0) ? width / colsOrWidth : width / colsOrWidth + 1;

        break;
    case SplitMode::Average:
        basicRowHeight = round(static_cast<double>(height) / rowsOrHeight);
        basicColWidth = round(static_cast<double>(width) / colsOrWidth);
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
    if ((seq & SplitSequence::Left) == SplitSequence::Left) {
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
    } else if ((seq & SplitSequence::Right) == SplitSequence::Right) {
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
                rects[i][j].setLeft(width - (cols - j) * basicColWidth);
                rects[i][j].setRight(width - (cols - j - 1) * basicColWidth);
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

[[nodiscard]] /* static */ bool ImageHandler::split(QImageReader &imgReader,
                                                    QVector<QImage> &output, const RectList &rects)
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
    for (qsizetype j{}; j != cRows; ++j)
        for (qsizetype k{}; k != cCols; ++k)
            output.push_back(img.copy(rects[j][k]));

    return true;
}

/* static */ void ImageHandler::rectsToLines(const RectList &rects, GraphicsView *parent)
{
    if (0 == rects.size() || 0 == rects[0].size())
        return;

    DraggableLine *line{};
    for (qsizetype i{}; i != rects.size() - 1; ++i) {
        line = new DraggableLine(parent);
        line->setScenePos(rects[i][0].bottom());
        line->show();
        parent->addDraggableLine(line);
        line = nullptr;
    }
    for (int i{}; i != rects[0].size() - 1; ++i) {
        line = new DraggableLine(Qt::Vertical, QPoint(RULER_SIZE, 0), parent);
        line->setScenePos(rects[0][i].right());
        line->show();
        parent->addDraggableLine(line);
        line = nullptr;
    }
}

/* static */ RectList ImageHandler::linesToRects(GraphicsView *parent)
{
    QVector<int> vx, vy;

    for (auto l : parent->getDraggableLines())
        if (l->getOrientation() == Qt::Horizontal)
            vy.push_back(l->scenePos());
        else
            vx.push_back(l->scenePos());
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

/* static */ void ImageHandler::drawGridLines(QPixmap *pixmap, const RectList &rects,
                                              const QColor &color, int size)
{
    if (rects.empty() || pixmap->isNull())
        return;

    QPainter p(pixmap);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(size);
    p.setPen(pen);

    for (int i{}; i != rects.size() - 1; ++i)
        p.drawLine(0, rects[i][0].bottom(), pixmap->width() - 1, rects[i][0].bottom());
    for (int j{}; j != rects[0].size() - 1; ++j)
        p.drawLine(rects[0][j].right(), 0, rects[0][j].right(), pixmap->height() - 1);
}

} // namespace Util
