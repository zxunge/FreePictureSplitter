# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_LIST_DIR}/ui")

qt_add_executable(FreePictureSplitter "")

set_target_properties(FreePictureSplitter PROPERTIES
    WIN32_EXECUTABLE TRUE
)

add_subdirectory(3rdparty)
add_subdirectory(app)
add_subdirectory(resources)
add_subdirectory(ui)
add_subdirectory(utils)

if(WITH_PCH)
    target_precompile_headers(FreePictureSplitter PRIVATE fps_pch.h)
endif()

target_link_libraries(FreePictureSplitter PRIVATE
    Qt::Core
    Qt::Widgets
    reflectcpp
)

# Deployment (with custom rules)
set(deploy_script "${CMAKE_CURRENT_BINARY_DIR}/deploy_fps.cmake")
if(UNIX OR (WIN32 AND (NOT DEPLOY_WINDOWS)))
    install(TARGETS FreePictureSplitter
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    file(GENERATE OUTPUT ${deploy_script} CONTENT "
        set(QT_DEPLOY_PLUGINS_DIR \"${CMAKE_INSTALL_LIBDIR}/plugins\")
        set(QT_DEPLOY_TRANSLATIONS_DIR \"${CMAKE_INSTALL_DATADIR}/fps/translations\")

        include(\"${QT_DEPLOY_SUPPORT}\")

        qt_deploy_runtime_dependencies(
            EXECUTABLE \"\${QT_DEPLOY_BIN_DIR}/$<TARGET_FILE_NAME:FreePictureSplitter>\"
        )")
else()    # Win32 && DEPLOY_WINDOWS
    install(TARGETS FreePictureSplitter
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
    file(GENERATE OUTPUT ${deploy_script} CONTENT "
        set(QT_DEPLOY_BIN_DIR \"\")
        set(QT_DEPLOY_PLUGINS_DIR \"plugins\")
        set(QT_DEPLOY_TRANSLATIONS_DIR \"translations\")

        include(\"${QT_DEPLOY_SUPPORT}\")

        qt_deploy_runtime_dependencies(
            EXECUTABLE \"\${QT_DEPLOY_BIN_DIR}/$<TARGET_FILE_NAME:FreePictureSplitter>\"
        )")
endif()

install(SCRIPT ${deploy_script})