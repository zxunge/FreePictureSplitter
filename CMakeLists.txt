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

option(BUILD_QT_GUI "Build FreePictureSplitter's Qt GUI (Default)." ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(BUILD_QT_GUI)
    message(STATUS "Qt GUI is enabled.")
    add_subdirectory(src)
endif()
