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
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    option(WITH_UBSAN "Build with UBSAN" OFF)
    option(WITH_TSAN "Build with TSAN" OFF)
    option(WITH_ASAN "Build with ASAN" OFF)
endif()
# Deployment Options
if(WIN32)
  option(DEPLOY_WINDOWS "Install exes/libs directly to prefix (no GNUInstallDirs)." ON)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Override variables
if(WIN32 AND DEPLOY_WINDOWS)
    set(CMAKE_INSTALL_BINDIR ".")
    set(CMAKE_INSTALL_LIBDIR ".")
    set(CMAKE_INSTALL_LIBEXECDIR ".")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Svg LinguistTools)

qt_standard_project_setup(
    REQUIRES 6.9
    I18N_TRANSLATED_LANGUAGES de fr ja pl tr uk zh_TW zh_CN it
)

configure_file(config/config.h.in config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(WITH_ASAN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
endif()

if(WITH_TSAN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread -Wl,-pie")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fPIC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread -fPIC")
endif()

if(WITH_UBSAN)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
endif()

if(WITH_QT_GUI)
    message(STATUS "Qt GUI is enabled.")
    add_subdirectory(src)
endif()
