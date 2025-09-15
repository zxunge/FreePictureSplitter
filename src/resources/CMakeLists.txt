target_sources(FreePictureSplitter PRIVATE fps.qrc resources.rc)
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
