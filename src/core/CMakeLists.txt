target_sources(FreePictureSplitter PRIVATE
    imagedocument.cpp
    imagedocument.h
    imagehandler.cpp
    imagehandler.h
)
target_include_directories(FreePictureSplitter PRIVATE ${CMAKE_CURRENT_LIST_DIR})