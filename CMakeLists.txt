# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

cmake_minimum_required(VERSION 3.20)

project(FreePictureSplitter
        VERSION 3.0.0
        LANGUAGES C CXX
        HOMEPAGE_URL https://github.com/zxunge/FreePictureSplitter
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

aux_source_directory(src SRC)
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "src/ui")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
if (Qt6_FOUND)
    message(STATUS "Found Qt " ${Qt6_VERSION})
elseif(Qt5_FOUND)
    message(STATUS "Found Qt " ${Qt5_VERSION})
else()
    message(FATAL_ERROR "Could not find Qt!")
endif()

configure_file(config/config.h.in config.h)

set(TS_FILES src/resources/i18n/FreePictureSplitter_zh_CN.ts)

set(PROJECT_SOURCES
        config/config.h.in
        src/main.cpp
        src/fpsmainwindow.h
        src/fpsmainwindow.cpp
        src/fpsimagehandler.h
        src/fpsimagehandler.cpp
        src/ui/fpsmainwindow.ui
        src/fpsbatchdialog.h
        src/fpsbatchdialog.cpp
        src/ui/fpsbatchdialog.ui
        src/fpsaboutdialog.h
        src/fpsaboutdialog.cpp
        src/fpssettingsdialog.h
        src/fpssettingsdialog.cpp
        src/ui/fpssettingsdialog.ui
        src/fpsfloatingline.h
        src/fpsfloatingline.cpp
        src/fpsprogressdialog.h
        src/fpsprogressdialog.cpp
        src/ui/fpsprogressdialog.ui
        src/fpsrulerbar.h
        src/fpsrulerbar.cpp
        src/resources/fps.qrc
        src/resources/resources.rc
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FreePictureSplitter
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FreePictureSplitter APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(FreePictureSplitter SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FreePictureSplitter
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_include_directories(FreePictureSplitter PRIVATE ${CMAKE_CURRENT_BINARY_DIR} src)

target_link_libraries(FreePictureSplitter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

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
