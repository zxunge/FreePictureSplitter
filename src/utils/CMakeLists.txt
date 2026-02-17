target_sources(FreePictureSplitter PRIVATE
    jsonconfigitems.h
    colors.h
    colors.cpp
    stdpaths.h
    stdpaths.cpp
    fonts.h
    leaveevent.h
    hovereventfilter.h
    fileinfo.h
    thememanager.cpp
    thememanager.h
)

if(WIN32 AND (NOT DEPLOY_WINDOWS))
    target_compile_definitions(FreePictureSplitter WIN32_GNU_DEPLOY)
endif()

