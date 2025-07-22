# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

# Generate translation files(.ts) from QT_I18N_TRANSLATED_LANGUAGES, like that in Qt6
function(gen_qt5_translations qm_files)
    set(one_value_args TARGET TS_FILE_DIR)
    cmake_parse_arguments(PARSE_ARGV 0 arg "" "${one_value_args}" "")
    # Build file names, use `TARGET` as the base name
    foreach(lang IN LISTS QT_I18N_TRANSLATED_LANGUAGES)
        set(ts_file "${arg_TS_FILE_DIR}/${arg_TARGET}_${lang}.ts")
        list(APPEND TS_FILES "${ts_file}")
    endforeach()
    set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "${arg_TS_FILE_DIR}")
    
    # Get source files
    get_target_property(prop_srcs ${arg_TARGET} SOURCES)
    foreach(src IN LISTS prop_srcs)
        list(APPEND sources ${src})
    endforeach()
    
    qt5_create_translation(_qm_files ${TS_FILES} ${sources} OPTIONS -no-obsolete)
    add_custom_target(update_translations DEPENDS ${TS_FILES})
    add_custom_target(release_translations DEPENDS ${_qm_files})
    set(${qm_files} ${_qm_files} PARENT_SCOPE)
endfunction()