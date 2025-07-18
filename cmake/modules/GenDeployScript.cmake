# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

# Custom deployment script generation
function(gen_deploy_script)
    set(one_value_args TARGET OUTPUT_SCRIPT WINDEPLOY)
    cmake_parse_arguments(PARSE_ARGV 0 arg "" "${one_value_args}" "")
    set(deploy_script "${CMAKE_CURRENT_BINARY_DIR}/deploy_${arg_TARGET}.cmake")
    # Project-specific content & Qt's generation
    if(${arg_WINDEPLOY})
        set(content "
set(QT_DEPLOY_PLUGINS_DIR \"plugins\")
set(QT_DEPLOY_TRANSLATIONS_DIR \"translations\")

include(\"${QT_DEPLOY_SUPPORT}\")

qt_deploy_runtime_dependencies(
    EXECUTABLE \"$<TARGET_FILE_NAME:${arg_TARGET}>\"
    GENERATE_QT_CONF
    NO_TRANSLATIONS
)
qt_deploy_translations(CATALOGS \"qtbase\")    ## It seems a bug that Qt cannot infer the catalogs automatically, so we need to specify it manually.
        ")
    else()
        set(content "
set(QT_DEPLOY_PLUGINS_DIR \"${CMAKE_INSTALL_LIBDIR}/plugins\")
set(QT_DEPLOY_TRANSLATIONS_DIR \"${CMAKE_INSTALL_DATADIR}/fps/translations\")

include(\"${QT_DEPLOY_SUPPORT}\")

qt_deploy_runtime_dependencies(
    EXECUTABLE \"\${QT_DEPLOY_BIN_DIR}/$<TARGET_FILE_NAME:${arg_TARGET}>\"
    GENERATE_QT_CONF
    NO_TRANSLATIONS
)
qt_deploy_translations(CATALOGS \"qtbase\")
        ")
    endif()
    
    file(GENERATE OUTPUT ${deploy_script} CONTENT ${content})
    set(${arg_OUTPUT_SCRIPT} ${deploy_script} PARENT_SCOPE)
endfunction()