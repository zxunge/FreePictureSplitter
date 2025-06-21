# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_LIST_DIR}/ui")

set(TS_FILES resources/i18n/FreePictureSplitter_zh_CN.ts)

set(QTGUI_SOURCES
        main.cpp
        fpsmainwindow.h
        fpsmainwindow.cpp
        fpsimagehandler.h
        fpsimagehandler.cpp
        ui/fpsmainwindow.ui
        fpsbatchdialog.h
        fpsbatchdialog.cpp
        ui/fpsbatchdialog.ui
        fpsaboutdialog.h
        fpsaboutdialog.cpp
        fpssettingsdialog.h
        fpssettingsdialog.cpp
        ui/fpssettingsdialog.ui
        fpsfloatingline.h
        fpsfloatingline.cpp
        fpsprogressdialog.h
        fpsprogressdialog.cpp
        ui/fpsprogressdialog.ui
        fpsrulerbar.h
        fpsrulerbar.cpp
        fpsgraphicsview.h
        fpsgraphicsview.cpp
        debugutil.h
        resources/fps.qrc
        resources/resources.rc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FreePictureSplitter
        MANUAL_FINALIZATION
        ${QTGUI_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FreePictureSplitter APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(FreePictureSplitter SHARED
            ${QTGUI_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FreePictureSplitter
            ${QTGUI_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_precompile_headers(FreePictureSplitter PRIVATE fps_pch.h)

target_include_directories(FreePictureSplitter PRIVATE .)

target_link_libraries(FreePictureSplitter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Gui)

set_target_properties(FreePictureSplitter PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.freepicturesplitter
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS FreePictureSplitter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FreePictureSplitter)
endif()
