# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

target_sources(FreePictureSplitter PRIVATE
    fpsaboutdialog.cpp
    fpsaboutdialog.h
    fpsbatchdialog.cpp
    fpsbatchdialog.h
    fpsfloatingline.cpp
    fpsfloatingline.h
    fpsgraphicsview.cpp
    fpsgraphicsview.h
    fpsimagehandler.cpp
    fpsimagehandler.h
    fpsmainwindow.cpp
    fpsmainwindow.h
    fpsprogressdialog.cpp
    fpsprogressdialog.h
    fpsrulerbar.cpp
    fpsrulerbar.h
    fpssettingsdialog.cpp
    fpssettingsdialog.h
    main.cpp
)

target_include_directories(FreePictureSplitter PRIVATE ${CMAKE_CURRENT_LIST_DIR})