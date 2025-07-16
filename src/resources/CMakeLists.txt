# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

set(TS_FILES
    ${CMAKE_CURRENT_LIST_DIR}/i18n/FreePictureSplitter_zh_CN.ts
)

target_sources(FreePictureSplitter PRIVATE
    ${TS_FILES}
    fps.qrc
    resources.rc
)

qt_add_translations(
    TARGETS FreePictureSplitter
    SOURCE_TARGETS FreePictureSplitter
    TS_FILES ${TS_FILES}
    LUPDATE_OPTIONS -no-obsolete
    QM_FILES_OUTPUT_VARIABLE QM_FILES
)