set(QWINDOWKIT_BUILD_EXAMPLES OFF CACHE BOOL "")
set(QWINDOWKIT_BUILD_STATIC ON CACHE BOOL "")

# Build qmsetup in place
set(qmsetup_source_dir ${CMAKE_CURRENT_LIST_DIR}/qwindowkit/qmsetup)

# Import install function
include("${qmsetup_source_dir}/cmake/modules/private/InstallPackage.cmake")

# Install package in place
set(qmsetup_package_path)
qm_install_package(qmsetup
    SOURCE_DIR ${qmsetup_source_dir}
    BUILD_TYPE Release
    RESULT_PATH qmsetup_package_path
)

# Update import path
set(qmsetup_DIR ${qmsetup_package_path} CACHE PATH "" FORCE)

add_subdirectory(qwindowkit)
add_subdirectory(qwindowkit/examples/shared/widgetframe)
add_subdirectory(reflect-cpp)
