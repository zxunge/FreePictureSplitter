# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_LIST_DIR}/ui")

if(Qt6_FOUND)
    qt_add_executable(FreePictureSplitter
        MANUAL_FINALIZATION
        ""
    )
else()
    add_executable(FreePictureSplitter "")
endif()

add_subdirectory(3rdparty)
add_subdirectory(app)
add_subdirectory(ui)
add_subdirectory(utils)
# Must be the last
add_subdirectory(resources)

if(WITH_PCH)
    target_precompile_headers(FreePictureSplitter PRIVATE fps_pch.h)
endif()

target_link_libraries(FreePictureSplitter PRIVATE
    Qt::Core
    Qt::Widgets
    reflectcpp
)

set_target_properties(FreePictureSplitter PROPERTIES
    WIN32_EXECUTABLE TRUE
)

# Deployment
include(GenDeployScript)
if(UNIX OR (WIN32 AND (NOT DEPLOY_WINDOWS)))
    install(TARGETS FreePictureSplitter
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION "${CMAKE_INSTALL_DATADIR}/licenses/fps")
    if(Qt6_FOUND)
        gen_deploy_script(
            TARGET FreePictureSplitter
            WINDEPLOY FALSE
            OUTPUT_SCRIPT deploy_script
        )
        install(SCRIPT ${deploy_script})
    endif()
    ## For Qt5, we need to deploy Qt files manually.
else()    # Win32 && DEPLOY_WINDOWS
    # A traditional Win32-like installation directory structure
    install(TARGETS FreePictureSplitter
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
    install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION "${CMAKE_INSTALL_PREFIX}")
    if(Qt6_FOUND)
        gen_deploy_script(
            TARGET FreePictureSplitter
            WINDEPLOY TRUE
            OUTPUT_SCRIPT deploy_script
        )
        install(SCRIPT ${deploy_script})
    endif()
    ## For Qt5, we need to deploy Qt files manually.
endif()

include(CPack)

if(Qt6_FOUND)
    qt_finalize_executable(FreePictureSplitter)
endif()
