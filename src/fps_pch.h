// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
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
