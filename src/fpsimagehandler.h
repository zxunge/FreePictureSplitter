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
    enum SplitSequence { TopLeft, TopRight, BottomLeft, BottomRight };  // We need to support both modes and sequences.
    Q_ENUM(SplitMode)
    Q_ENUM(SplitSequence)

public:
    // It's important to know that both our `QVector<QVector<QRect>> rect' and `QStringList list' have an 'accessing' sequence of TopLeft -> BottomRight.
    static QVector<QVector<QRect>>
    getSubRects(int width,
                int height,
                int rowsOrHeight,
                int colsOrWidth,
                SplitMode mode = Average,
                SplitSequence seq = TopLeft);
    
    static bool split(const QImage &img,
                      QVector<QImage> &output,
                      const QVector<QVector<QRect>> &rects);

    static QStringList getOutputList(const QString &fileName, int rows, int cols);

signals:
    void proceed(int progress);
};

#endif // FPSIMAGEHANDLER_H
