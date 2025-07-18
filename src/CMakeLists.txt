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

# Deployment
## Custom deployment script
set(deploy_script "${CMAKE_CURRENT_BINARY_DIR}/deploy_fps.cmake")
include(GenDeployScript)
if(UNIX OR (WIN32 AND (NOT DEPLOY_WINDOWS)))
    install(TARGETS FreePictureSplitter
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    gen_deploy_script(
        TARGET FreePictureSplitter
        WINDEPLOY FALSE
        SCRIPT ${deploy_script}
    )
else()    # Win32 && DEPLOY_WINDOWS
    install(TARGETS FreePictureSplitter
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
    gen_deploy_script(
        TARGET FreePictureSplitter
        WINDEPLOY TRUE
        SCRIPT ${deploy_script}
    )
endif()

install(SCRIPT ${deploy_script})