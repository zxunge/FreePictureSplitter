target_sources(FreePictureSplitter PRIVATE
    jsonconfigitems.h
    debugutil.h
    colors.h
    colors.cpp
    skins.h
    skins.cpp
    stdpaths.h
    stdpaths.cpp
)

target_include_directories(FreePictureSplitter PRIVATE ${CMAKE_CURRENT_LIST_DIR})

if(WIN32 AND (NOT DEPLOY_WINDOWS))
    target_compile_definitions(FreePictureSplitter WIN32_GNU_DEPLOY)
endif()