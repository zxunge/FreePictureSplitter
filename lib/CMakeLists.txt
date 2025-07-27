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
