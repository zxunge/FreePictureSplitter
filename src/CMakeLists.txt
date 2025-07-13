# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_LIST_DIR}/ui")

set(TS_FILES
    resources/i18n/FreePictureSplitter_zh_CN.ts
)

set(UI_FILES
    ui/fpsmainwindow.ui
    ui/fpsbatchdialog.ui
    ui/fpssettingsdialog.ui
    ui/fpsprogressdialog.ui
)

set(RC_FILES
    resources/fps.qrc
    resources/resources.rc
)

set(APP_SRCS
    app/fpsaboutdialog.cpp
    app/fpsaboutdialog.h
    app/fpsbatchdialog.cpp
    app/fpsbatchdialog.h
    app/fpsfloatingline.cpp
    app/fpsfloatingline.h
    app/fpsgraphicsview.cpp
    app/fpsgraphicsview.h
    app/fpsimagehandler.cpp
    app/fpsimagehandler.h
    app/fpsmainwindow.cpp
    app/fpsmainwindow.h
    app/fpsprogressdialog.cpp
    app/fpsprogressdialog.h
    app/fpsrulerbar.cpp
    app/fpsrulerbar.h
    app/fpssettingsdialog.cpp
    app/fpssettingsdialog.h
    app/main.cpp
)

set(UTILS_SRCS
    utils/jsonconfigitems.h
    utils/debugutil.h
    utils/colors.h
    utils/colors.cpp
    utils/skins.h
    utils/skins.cpp
)

set(QTGUI_SOURCES ${APP_SRCS} ${UTILS_SRCS} ${UI_FILES} ${RC_FILES} ${TS_FILES})

if(Qt6_FOUND)
    qt_add_executable(FreePictureSplitter
        MANUAL_FINALIZATION
        ${QTGUI_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FreePictureSplitter APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(FreePictureSplitter SHARED
            ${QTGUI_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FreePictureSplitter
            ${QTGUI_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

add_subdirectory(3rdparty)

if(WITH_PCH)
    target_precompile_headers(FreePictureSplitter PRIVATE fps_pch.h)
endif()

target_include_directories(FreePictureSplitter PRIVATE
    app
    utils
)

target_link_libraries(FreePictureSplitter PRIVATE
    Qt::Core
    Qt::Widgets
    reflectcpp
)

set_target_properties(FreePictureSplitter PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.freepicturesplitter
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS FreePictureSplitter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
)

# Install resources
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/resources/skins DESTINATION ${CMAKE_INSTALL_PREFIX})

if(Qt6_FOUND)
    qt_finalize_executable(FreePictureSplitter)
endif()
