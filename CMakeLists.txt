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

option(WITH_QT_GUI "Build FreePictureSplitter's Qt GUI (Default)." ON)
option(WITH_PCH "Using Pre-compiled Headers(PCH)." ON)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    option(WITH_UBSAN "Build with UBSAN" OFF)
    option(WITH_TSAN "Build with TSAN" OFF)
    option(WITH_ASAN "Build with ASAN" OFF)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt6 QUIET COMPONENTS Core Widgets LinguistTools)
if (NOT Qt6_FOUND)
    # We need at least Qt 5.15 to enable some CMake and App features.
    find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets LinguistTools)
    if(Qt5_FOUND)
        message(STATUS "Found Qt " ${Qt5_VERSION})
    else()
        message(FATAL_ERROR "Could not find Qt (need version >= 5.15)!")
    endif()
else()
    message(STATUS "Found Qt " ${Qt6_VERSION})
endif()

configure_file(config/config.h.in config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(WITH_ASAN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
endif()

if(WITH_TSAN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread -Wl,-pie")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fPIC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread -fPIC")
endif()

if(WITH_UBSAN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
endif()

if(WITH_QT_GUI)
    message(STATUS "Qt GUI is enabled.")
    add_subdirectory(src)
endif()
