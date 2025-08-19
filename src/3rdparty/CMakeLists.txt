set(QWINDOWKIT_BUILD_EXAMPLES OFF CACHE BOOL "")
set(QWINDOWKIT_BUILD_STATIC ON CACHE BOOL "")

add_subdirectory(qwindowkit/qmsetup)
add_subdirectory(qwindowkit)
add_subdirectory(qwindowkit/examples/shared/widgetframe)
add_subdirectory(reflect-cpp)
