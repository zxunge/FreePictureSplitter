cmake_minimum_required(VERSION 3.22)

project(FreePictureSplitter
        VERSION 4.0.0
        LANGUAGES C CXX
        DESCRIPTION "A fast, small, and easy-to-use picture splitter!"
        HOMEPAGE_URL https://github.com/zxunge/FreePictureSplitter
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

# We do not support other platforms yet.
if((NOT UNIX) AND (NOT WIN32))
    message(FATAL_ERROR "Could not recognize the platform!")
endif()

# Build Options
option(WITH_QT_GUI "Build FreePictureSplitter's Qt GUI (default)." ON)
option(WITH_PCH "Use pre-compiled headers (PCH)." ON)
# Deployment Options
if(WIN32)
  option(DEPLOY_WINDOWS "Install exes/libs directly to prefix (no GNUInstallDirs)." ON)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(QT_NO_PRIVATE_MODULE_WARNING ON CACHE BOOL "")

# Override variables
if(WIN32 AND DEPLOY_WINDOWS)
    set(CMAKE_INSTALL_BINDIR "." CACHE PATH "")
    set(CMAKE_INSTALL_LIBDIR "." CACHE PATH "")
    set(CMAKE_INSTALL_LIBEXECDIR "." CACHE PATH "")
endif()

find_package(QT NAMES Qt6 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS
    Core Gui Widgets Concurrent LinguistTools Svg
    CorePrivate GuiPrivate WidgetsPrivate
)

qt_standard_project_setup(
    REQUIRES 6.9
    I18N_TRANSLATED_LANGUAGES de fr ja pl tr uk zh_TW zh_CN it
)

message(STATUS "Found Qt " ${Qt6_VERSION})

configure_file(config/config.h.in config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(WITH_QT_GUI)
    message(STATUS "Qt GUI is enabled.")
    add_subdirectory(src)
    add_subdirectory(lib)
endif()

message(STATUS "Configuration Options Summary --
    Target system:          ${CMAKE_SYSTEM}
    Compiler:
        Build type:         ${CMAKE_BUILD_TYPE}
        C compiler:         ${CMAKE_C_COMPILER}
        C++ compiler:       ${CMAKE_CXX_COMPILER}
        Qt version:         ${QT_DEFAULT_MAJOR_VERSION}
        Qt toolchain:       ${QT_QMAKE_EXECUTABLE}"
)
