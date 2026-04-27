add_subdirectory(reflect-cpp)
add_subdirectory(qlementine)

if(MSVC)
    # Fix issue https://github.com/getml/reflect-cpp/issues/560
    target_compile_definitions(reflectcpp PUBLIC NOMINMAX)
endif()
