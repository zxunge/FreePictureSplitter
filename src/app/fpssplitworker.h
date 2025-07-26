/*
 * This file is a part of FreePictureSplitter, a fast and easy-to-use picture splitter.
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
    /// \param quality        Output images' quality (default is 100) (only some formats, \see {
    /// https://doc.qt.io/qt-6/qimagewriter.html#setQuality }.
    /// \details Designed to handle the most essential part of image splitting, doSplit() will only
    /// include several lines of code.
    ///
    void doSplit(const QStringList &files, const QVector<QStringList> &outputs,
                 const QVector<QVector<QImage>> &images, const QString &outPath,
                 const QString &format, const double scaleFactor = 1.0, const int quality = 100);

signals:
    void error(const QString &fileName);
    void ready();
};

#endif // FPSSPLITWORKER_H
