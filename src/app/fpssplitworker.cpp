// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpssplitworker.h"

#include <QImageWriter>

void fpsSplitWorker::doSplit(const QString &files,
                             const QStringList &outputs,
                             const QVector<QImage> &images,
                             const QString &outPath,
                             const QString &format,
                             const double scaleFactor,
                             const int quality)
{
    QImageWriter writer;

    for (int i{}; i != images.size(); ++i) {

        writer.setFileName(outPath+ '/' + outputs[i]);
        writer.setFormat(format.toUtf8());
        writer.setQuality(quality);
        if (!writer.write(images[i].scaled(
                    images[i].width() * scaleFactor,
                    images[i].height() * scaleFactor,
                    Qt::IgnoreAspectRatio, Qt::SmoothTransformation))) {

            break;
        }
    }
}
