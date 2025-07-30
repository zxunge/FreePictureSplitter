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
#include "debugutil.h"

#include <QImageWriter>
#include <QThread>

using namespace Qt::Literals::StringLiterals;

void fpsSplitWorker::doSplit()
{
    Q_ASSERT(m_images.size() == m_outputs.size());
    Q_ASSERT(m_outPath.size() == m_outputs.size());

    QImageWriter writer;
    int count{};

    for (qsizetype i{}; i != m_images.size(); ++i) {
        // Each source image file
        for (int j{}; j != m_images[i].size(); ++j) {
            // Was requested to be interrupted?
            if (QThread::currentThread()->isInterruptionRequested()) {
                fpsDebugStr("Interruption Requested!");
                Q_EMIT ready();
                return;
            }
            // Each image in the corresponding list of each file
            writer.setFileName(m_outPath[i] + u"/"_s + m_outputs[i][j]);
            writer.setFormat(m_format.toUtf8());
            writer.setQuality(m_quality);
            if (!writer.write(m_images[i][j].scaled(m_images[i][j].width() * m_scaleFactor,
                                                    m_images[i][j].height() * m_scaleFactor,
                                                    Qt::IgnoreAspectRatio,
                                                    Qt::SmoothTransformation))) {
                Q_EMIT error(tr("Error saving to file: \'%1\': %2.")
                                     .arg(writer.fileName(), writer.errorString()));
                return;
            }
            Q_EMIT proceed(++count);
        }
    }
    Q_EMIT ready();
}
