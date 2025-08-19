set(QWINDOWKIT_BUILD_EXAMPLES OFF CACHE BOOL "")
set(QWINDOWKIT_BUILD_STATIC ON CACHE BOOL "")

# Reset them here
set(CMAKE_INSTALL_BINDIR "bin")
set(CMAKE_INSTALL_LIBDIR "lib")
set(CMAKE_INSTALL_LIBEXECDIR "libexec")

add_subdirectory(qwindowkit)
add_subdirectory(qwindowkit/examples/shared/widgetframe)
add_subdirectory(reflect-cpp)

