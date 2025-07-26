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

#include "fpssplitworker.h"

#include <QImageWriter>

using namespace Qt::Literals::StringLiterals;

void fpsSplitWorker::doSplit(const QStringList &files, const QVector<QStringList> &outputs,
                             const QVector<QVector<QImage>> &images, const QString &outPath,
                             const QString &format, const double scaleFactor, const int quality)
{
    if (!(files.size() == images.size() && files.size() == outputs.size()))
        return;

    QImageWriter writer;

    for (qsizetype i{}; i != files.size(); ++i) {
        // Each file
        const QString file{ files[i] };
        for (int j{}; j != images[i].size(); ++j) {
            // Each image in the corresponding list of each file
            writer.setFileName(outPath + u"/"_s + outputs[i][j]);
            writer.setFormat(format.toUtf8());
            writer.setQuality(quality);
            if (!writer.write(images[i][j].scaled(
                        images[i][j].width() * scaleFactor, images[i][j].height() * scaleFactor,
                        Qt::IgnoreAspectRatio, Qt::SmoothTransformation))) {
                Q_EMIT error(writer.fileName());
                return;
            }
        }
    }
    Q_EMIT ready();
}
