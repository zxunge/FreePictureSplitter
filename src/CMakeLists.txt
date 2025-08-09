list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_LIST_DIR}/ui")

qt_add_executable(FreePictureSplitter "")

set_target_properties(FreePictureSplitter PROPERTIES
    WIN32_EXECUTABLE TRUE
)

add_subdirectory(3rdparty)
add_subdirectory(app)
add_subdirectory(resources)
add_subdirectory(ui)
add_subdirectory(utils)
add_subdirectory(widgets)

if(WITH_PCH)
    target_precompile_headers(FreePictureSplitter PRIVATE fps_pch.h)
endif()

target_link_libraries(FreePictureSplitter PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Svg
    Qt::Concurrent
    reflectcpp
    FramelessWindow
)

# Deployment
## Custom deployment script
include(GenDeployScript)
if(UNIX OR (WIN32 AND (NOT DEPLOY_WINDOWS)))
    install(TARGETS FreePictureSplitter
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION "${CMAKE_INSTALL_DATADIR}/licenses/fps")
    gen_deploy_script(
        TARGET FreePictureSplitter
        WINDEPLOY FALSE
        OUTPUT_SCRIPT deploy_script
    )
else()    # Win32 && DEPLOY_WINDOWS
    install(TARGETS FreePictureSplitter
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    )
    install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION "${CMAKE_INSTALL_PREFIX}")
    gen_deploy_script(
        TARGET FreePictureSplitter
        WINDEPLOY TRUE
        OUTPUT_SCRIPT deploy_script
    )
endif()
install(SCRIPT ${deploy_script})
# Note that we need to deploy Dr.MinGW dlls manually,
# including exchndl.dll, mgwhelp.dll, dbghelp.dll, dbgcore.dll, symsrv.dll, and symsrv.yes.
include(CPack)
