cmake_minimum_required(VERSION 3.25)
set(PROJECT_NAME FreePictureSplitter)
project(${PROJECT_NAME} VERSION 2.0.0
                        HOMEPAGE_URL https://github.com/zxunge/FreePictureSplitter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY bin)

set(PROJECT_STATUS Alpha)

configure_file(
    "${PROJECT_SOURCE_DIR}/include/config.h.in" 
    "${PROJECT_SOURCE_DIR}/include/config.h"
)

include_directories(include ui)
add_executable(
    ${PROJECT_NAME} 
    "${PROJECT_SOURCE_DIR}/src/app.cpp" 
    "${PROJECT_SOURCE_DIR}/src/fpsImage.cpp" 
    "${PROJECT_SOURCE_DIR}/src/imagepanel.cpp" 
    "${PROJECT_SOURCE_DIR}/ui/MainFrame.cpp" 
    "${PROJECT_SOURCE_DIR}/ui/fpsBatchSplitDialog.cpp"
    "${PROJECT_SOURCE_DIR}/ui/UiBase.cpp"
    "${PROJECT_SOURCE_DIR}/ui/bitmaps.cpp"
    "${PROJECT_SOURCE_DIR}/res/win_resources.rc"
)
find_package(wxWidgets COMPONENTS core base html aui adv xml xrc)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
endif()