configure_file(resources.rc.in resources.rc ESCAPE_QUOTES @ONLY)
file(COPY_FILE ${CMAKE_CURRENT_SOURCE_DIR}/fps.ico ${CMAKE_CURRENT_BINARY_DIR}/fps.ico)
target_sources(FreePictureSplitter PRIVATE fps.qrc ${CMAKE_CURRENT_BINARY_DIR}/resources.rc)
qt_add_translations(
    TARGETS FreePictureSplitter
    LUPDATE_OPTIONS -no-obsolete
    TS_OUTPUT_DIRECTORY i18n
    QM_FILES_OUTPUT_VARIABLE QM_FILES
)

# Install resources
if(UNIX OR (WIN32 AND (NOT DEPLOY_WINDOWS)))
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/skins DESTINATION ${CMAKE_INSTALL_DATADIR}/fps)
    install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/fps/translations)
else()
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/skins DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/translations)
endif()
