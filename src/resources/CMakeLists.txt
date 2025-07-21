# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

target_sources(FreePictureSplitter PRIVATE
    fps.qrc
    resources.rc
)

qt_add_translations(
    TARGETS FreePictureSplitter
    LUPDATE_OPTIONS -no-obsolete
    TS_OUTPUT_DIRECTORY i18n
    QM_FILES_OUTPUT_VARIABLE QM_FILES
)

# Install resources
if(UNIX OR (WIN32 AND (NOT DEPLOY_WINDOWS)))
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/skins DESTINATION ${CMAKE_INSTALL_DATADIR}/fps)
    install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/fps/translations)
else()
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/skins DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/translations)
endif()