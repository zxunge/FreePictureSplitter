//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FPSIMAGEHANDLER_H
#define FPSIMAGEHANDLER_H

#include <QString>
#include <QRect>
#include <QImage>
#include <QVector>
#include <QObject>
#include <cstddef>

class fpsImageHandler : public QObject
{
    Q_OBJECT
public:
    enum SplitMode { Size, Average };
    enum SplitSequence { HoriLeft, HoriRight, VertLeft, VertRight };  // We need to support both modes and sequences.
    Q_ENUM(SplitMode)
    Q_ENUM(SplitSequence)

public:
    static QVector<QVector<QRect>>
    getSubRects(int width,
                int height,
                int rowsOrHeight,
                int colsOrWidth,
                SplitMode mode = Average,
                SplitSequence seq = HoriLeft);

    static bool split(const QImage &img,
                      QVector<QImage> &output,
                      const QVector<QVector<QRect>> &rects);

    static QStringList getOutputList(const QString &fileName, int rows, int cols);

signals:
    void proceed(size_t progress);
};

#endif // FPSIMAGEHANDLER_H
