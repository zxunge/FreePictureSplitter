configure_file(app_version.h.in app_version.h ESCAPE_QUOTES @ONLY)
target_include_directories(FreePictureSplitter PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_sources(FreePictureSplitter PRIVATE
    main.cpp
)
