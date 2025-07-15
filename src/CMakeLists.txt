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
    utils/stdpaths.h
    utils/stdpaths.cpp
)

set(QTGUI_SOURCES ${APP_SRCS} ${UTILS_SRCS} ${UI_FILES} ${RC_FILES} ${TS_FILES})

qt_add_executable(FreePictureSplitter
    MANUAL_FINALIZATION
    ${QTGUI_SOURCES}
)

# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
set_property(TARGET FreePictureSplitter
    APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

qt_add_translations(
    TARGETS FreePictureSplitter
    SOURCE_TARGETS FreePictureSplitter
    TS_FILES ${TS_FILES}
    LUPDATE_OPTIONS -no-obsolete
    QM_FILES_OUTPUT_VARIABLE QM_FILES
)

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
    qlementine
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
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install resources
if(UNIX)
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/resources/skins DESTINATION ${CMAKE_INSTALL_DATADIR}/fps)
    install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/fps/translations)
elseif(WIN32)
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/resources/skins DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/translations)
endif()

qt_generate_deploy_app_script(
    TARGET FreePictureSplitter
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

qt_finalize_executable(FreePictureSplitter)
