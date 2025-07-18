# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

# Custom deployment script generation
function(gen_deploy_script)
    set(one_value_args TARGET SCRIPT WINDEPLOY)
    cmake_parse_arguments(PARSE_ARGV 0 arg "${options}" "${one_value_args}" "")
    # Project-specific content + Qt's generation
    if(${arg_WINDEPLOY})
        set(header "
            set(QT_DEPLOY_PLUGINS_DIR \"plugins\")
            set(QT_DEPLOY_TRANSLATIONS_DIR \"translations\")
        ")
    else()
        set(header "
            set(QT_DEPLOY_PLUGINS_DIR \"${CMAKE_INSTALL_LIBDIR}/plugins\")
            set(QT_DEPLOY_TRANSLATIONS_DIR \"${CMAKE_INSTALL_DATADIR}/fps/translations\")
        ")
    endif()
    
    qt_generate_deploy_app_script(
        TARGET ${arg_TARGET}
        OUTPUT_SCRIPT deploy_script
    )
    
    file(READ ${deploy_script} origin)
    string(APPEND header ${origin})
    file(WRITE ${arg_SCRIPT} ${header})
endfunction()