# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_LIST_DIR}/ui")

qt_add_executable(FreePictureSplitter
    MANUAL_FINALIZATION
    ""
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
target_link_libraries(FreePictureSplitter PRIVATE Qt6::Core)

set_target_properties(FreePictureSplitter PROPERTIES
    WIN32_EXECUTABLE TRUE
)

if(UNIX)
    install(TARGETS FreePictureSplitter
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
else()
    if(NOT DEPLOY_WINDOWS)
        target_compile_definitions(FreePictureSplitter PRIVATE WIN32_GNU_DEPLOY)
        install(TARGETS FreePictureSplitter
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
    else()
        install(TARGETS FreePictureSplitter
            LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
            RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
        )
    endif()
endif()

qt_generate_deploy_app_script(
    TARGET FreePictureSplitter
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

qt_finalize_executable(FreePictureSplitter)
