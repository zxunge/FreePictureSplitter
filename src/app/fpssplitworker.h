// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSSPLITWORKER_H
#define FPSSPLITWORKER_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include <QImage>

/*!
 * \brief The fpsSplitWorker class is to process the core images-splitting tasks.
 * \class fpsSplitWorker
 * \inherits QObject
 * \include fpssplitworker.h
 */
class fpsSplitWorker : public QObject
{
    Q_OBJECT
public slots:
    ///
    /// \brief doSplit
    /// \param files          A string list of source files' paths.
    /// \param outputs        A string list of the output files' full names.
    /// \param images         A list of <em>QImage</em>s to save.
    /// \param outPath        A QString indicating the output directory.
    /// \param format         Output pictures' format.
    /// \param scaleFactor    Factor used to scale the output images (default is 1.0).
    /// \param quality        Output images' quality (default is 100) (only some formats, \see { https://doc.qt.io/qt-6/qimagewriter.html#setQuality }.
    /// \details Designed to handle the most essential part of image splitting, doSplit() will only include several lines of code.
    ///
    void doSplit(const QString &files,
                 const QStringList &outputs,
                 const QVector<QImage> &images,
                 const QString &outPath,
                 const QString &format,
                 const double scaleFactor = 1.0,
                 const int quality = 100);

signals:
};

#endif // FPSSPLITWORKER_H
