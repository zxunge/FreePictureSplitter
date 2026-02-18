# Custom deployment script generation
function(gen_deploy_script)
    set(one_value_args TARGET OUTPUT_SCRIPT WINDEPLOY)
    cmake_parse_arguments(PARSE_ARGV 0 arg "" "${one_value_args}" "")
    set(deploy_script "${CMAKE_CURRENT_BINARY_DIR}/deploy_${arg_TARGET}_$<CONFIG>.cmake")
    # Project-specific content & Qt's generation
    if(${arg_WINDEPLOY})
        set(content "
set(QT_DEPLOY_PLUGINS_DIR \"plugins\")
set(QT_DEPLOY_TRANSLATIONS_DIR \"translations\")

include(\"${QT_DEPLOY_SUPPORT}\")
## It seems a bug that Qt cannot infer the catalogs automatically, so we need to specify it manually.
## Copied from Qt's auto-generated deploy_xxx.cmake.
set(__QT_DEPLOY_I18N_CATALOGS \"qtbase\")
qt_deploy_runtime_dependencies(
    EXECUTABLE \"$<TARGET_FILE_NAME:${arg_TARGET}>\"
    GENERATE_QT_CONF
)
        ")
    else()
        set(content "
set(QT_DEPLOY_PLUGINS_DIR \"${CMAKE_INSTALL_LIBDIR}/plugins\")
set(QT_DEPLOY_TRANSLATIONS_DIR \"${CMAKE_INSTALL_DATADIR}/fps/translations\")

include(\"${QT_DEPLOY_SUPPORT}\")
set(__QT_DEPLOY_I18N_CATALOGS \"qtbase\")
qt_deploy_runtime_dependencies(
    EXECUTABLE \"\${QT_DEPLOY_BIN_DIR}/$<TARGET_FILE_NAME:${arg_TARGET}>\"
)

# Manually generate our custom qt.conf file
qt6_deploy_qt_conf(\"\${QT_DEPLOY_PREFIX}/\${QT_DEPLOY_BIN_DIR}/qt.conf\"
    PREFIX \"..\"
    BIN_DIR \"\${QT_DEPLOY_BIN_DIR}\"
    LIB_DIR \"\${QT_DEPLOY_LIB_DIR}\"
    PLUGINS_DIR \"\${QT_DEPLOY_PLUGINS_DIR}\"
    DATA_DIR \"share\"
)
        ")
    endif()
    
    file(GENERATE OUTPUT ${deploy_script} CONTENT ${content})
    set(${arg_OUTPUT_SCRIPT} ${deploy_script} PARENT_SCOPE)
endfunction()
