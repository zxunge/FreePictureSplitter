# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
# Copyright (c) 2024 2025 zxunge
# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
# SPDX-License-Identifier: GPL-3.0-or-later

# Generate translation files(.ts) from QT_I18N_TRANSLATED_LANGUAGES, like that in Qt6
function(qt5_gen_translations _qm_files)
    set(one_value_args TARGET TS_FILES_DIR)
    cmake_parse_arguments(PARSE_ARGV 0 arg "" "${one_value_args}" "")
    
    set(TS_FILES)
    set(TGT_SRCS)
    set(QM_FILES)
    # Build file names, use `TARGET` as the base name
    foreach(lang IN LISTS QT_I18N_TRANSLATED_LANGUAGES)
        set(ts_file "${arg_TS_FILES_DIR}/${arg_TARGET}_${lang}.ts")
        list(APPEND TS_FILES "${ts_file}")
    endforeach()
    
    # Get source files
    get_target_property(prop_srcs ${arg_TARGET} SOURCES)
    foreach(src IN LISTS prop_srcs)
        list(APPEND TGT_SRCS ${src})
    endforeach()
    
    # Use our custom script to create translations,
    # since our targets are not created in the same directory
    # as where the `add_custom_command()` is called.
    # Copied and modified from Qt5LinguistToolsMacros.cmake.
    set(_extensions_list "*.java;*.jui;*.ui;*.c;*.c++;*.cc;*.cpp;*.cxx;*.ch;*.h;*.h++;*.hh;*.hpp;*.hxx;*.js;*.qs;*.qml;*.qrc")
    foreach(_ts_file ${TS_FILES})
        get_filename_component(_ts_name ${_ts_file} NAME)
        if(TGT_SRCS)
            # make a list file to call lupdate on, so we don't make our commands too
            # long for some systems
            set(_ts_lst_file "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_ts_name}_lst_file")
            set(_lst_file_srcs)
            set(_dependencies)
            foreach(_lst_file_src ${TGT_SRCS})
                set(_lst_file_srcs "${_lst_file_src}\n${_lst_file_srcs}")
                if(IS_DIRECTORY ${_lst_file_src})
                    list(TRANSFORM _extensions_list PREPEND "${_lst_file_src}/" OUTPUT_VARIABLE _directory_glob)
                    file(GLOB_RECURSE _directory_contents CONFIGURE_DEPENDS ${_directory_glob})
                    list(APPEND _dependencies ${_directory_contents})
                else()
                    list(APPEND _dependencies "${_lst_file_src}")
                endif()
            endforeach()
            
            get_directory_property(_inc_DIRS INCLUDE_DIRECTORIES)
            foreach(_pro_include ${_inc_DIRS})
                get_filename_component(_abs_include "${_pro_include}" ABSOLUTE)
                set(_lst_file_srcs "-I${_pro_include}\n${_lst_file_srcs}")
           endforeach()
           file(WRITE ${_ts_lst_file} "${_lst_file_srcs}")
        endif()
        
        add_custom_command(OUTPUT ${_ts_file}
            COMMAND ${Qt5_LUPDATE_EXECUTABLE}
            ARGS -no-obsolete "@${_ts_lst_file}" -ts ${_ts_file}
            DEPENDS ${_dependencies}
            VERBATIM
        )
    endforeach()
    qt5_add_translation(QM_FILES ${TS_FILES})
    
    add_custom_target(update_translations DEPENDS ${TS_FILES})
    add_custom_target(release_translations DEPENDS ${QM_FILES})
    add_dependencies(release_translations update_translations)
    set(${_qm_files} ${QM_FILES} PARENT_SCOPE)
endfunction()