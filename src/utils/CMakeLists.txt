# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

target_sources(FreePictureSplitter PRIVATE
    jsonconfigitems.h
    debugutil.h
    colors.h
    colors.cpp
    skins.h
    skins.cpp
    stdpaths.h
    stdpaths.cpp
)

target_include_directories(FreePictureSplitter PRIVATE ${CMAKE_CURRENT_LIST_DIR})

if(WIN32 AND (NOT DEPLOY_WINDOWS))
    target_compile_definitions(FreePictureSplitter WIN32_GNU_DEPLOY)
endif()