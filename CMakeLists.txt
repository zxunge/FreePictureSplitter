cmake_minimum_required(VERSION 3.20)

project(FreePictureSplitter VERSION 3.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(src SRC)
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "src/ui")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
if (Qt6_FOUND)
    message("-- Found Qt " ${Qt6_VERSION})
elseif(Qt5_FOUND)
    message("-- Found Qt " ${Qt5_VERSION})
else()
    message(FATAL_ERROR "-- Could not find Qt!")
endif()

set(TS_FILES src/resources/FreePictureSplitter_zh_CN.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/fpsmainwindow.h
        src/fpsmainwindow.cpp
        src/fpsmainwindow.ui
        src/3rdparty/QJSONConfig/QJSONConfig/qjsonconfig.cpp
        src/3rdparty/QJSONConfig/QJSONConfig/qjsonconfig.h
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FreePictureSplitter
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FreePictureSplitter APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(FreePictureSplitter SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FreePictureSplitter
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

include_directories(src/3rdparty/QJSONConfig/QJSONConfig)

set(QWINDOWKIT_BUILD_EXAMPLES OFF CACHE BOOL "")
set(QWINDOWKIT_BUILD_STATIC ON CACHE BOOL "")

add_subdirectory(src/3rdparty/qmsetup)
add_subdirectory(src/3rdparty/qwindowkit)

# target_link_libraries(FreePictureSplitter PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

qm_configure_target(${PROJECT_NAME}
    QT_LINKS Widgets
    LINKS QWKCore QWKWidgets # WidgetFrame
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
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FreePictureSplitter)
endif()
