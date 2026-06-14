if(MINGW)
    # Install ExcHndl dlls
    if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "AMD64")
        list(APPEND exchndl_dlls
            "exchndl/amd64/exchndl.dll"
            "exchndl/amd64/mgwhelp.dll"
            "exchndl/amd64/dbghelp.dll"
            "exchndl/amd64/dbgcore.dll"
            "exchndl/amd64/symsrv.dll"
            "exchndl/amd64/symsrv.yes"
        )
    else()
        list(APPEND exchndl_dlls
            "exchndl/x86/exchndl.dll"
            "exchndl/x86/mgwhelp.dll"
            "exchndl/x86/dbghelp.dll"
            "exchndl/x86/dbgcore.dll"
            "exchndl/x86/symsrv.dll"
            "exchndl/x86/symsrv.yes"
        )
    endif()
    install(FILES ${exchndl_dlls} DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

set(QWINDOWKIT_BUILD_EXAMPLES OFF CACHE BOOL "")
set(QWINDOWKIT_BUILD_STATIC ON CACHE BOOL "")

if(DEPLOY_WINDOWS)
    # Reset them here to avoid errors
    set(CMAKE_INSTALL_BINDIR "bin")
    set(CMAKE_INSTALL_LIBDIR "lib")
    set(CMAKE_INSTALL_LIBEXECDIR "libexec")
endif()

add_subdirectory(qwindowkit)
add_subdirectory(qwindowkit/examples/shared/widgetframe)
add_subdirectory(tools)
add_subdirectory(reflect-cpp)
add_subdirectory(qlementine)

if(MSVC)
    # Fix issue https://github.com/getml/reflect-cpp/issues/560
    target_compile_definitions(reflectcpp PUBLIC NOMINMAX)
endif()
