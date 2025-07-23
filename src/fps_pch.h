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

/***** Pre-compiled header *****/

#ifndef FPS_PCH_H
#define FPS_PCH_H

#ifdef __cplusplus
// C++ includes
#  include <cmath>
#  include <algorithm>
#  include <array>
#  include <string>

// Qt includes
#  include <QWidget>
#  include <QApplication>
#  include <QDialog>
#  include <QGraphicsView>
#  include <QMainWindow>
#  include <QPixmap>
#  include <QImage>
#  include <QPainter>
#  include <QFileDialog>
#  include <QMessageBox>
#  include <QMouseEvent>
#  include <QPaintEvent>
#  include <QDesktopServices>
#  include <QUrl>
#  include <QImageReader>
#  include <QImageWriter>
#  include <QStringLiteral>
#  include <QtCore/qdebug.h>
#  include <QtCore/qvector.h>
#  include <QtCore/qpointer.h>
#  include <QtCore/qstring.h>
#  include <QtCore/qrect.h>
#  include <QtCore/qobject.h>
#  if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#    include <QtWidgets>
#  endif

// Project includes
#  include "debugutil.h"

// Third-party includes
#  include <rfl/json.hpp>
#endif // __cplusplus

#endif // FPS_PCH_H
