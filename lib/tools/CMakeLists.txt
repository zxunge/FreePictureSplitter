qt_add_library(TitleBar STATIC
    titlebar.cpp
    titlebar.h
)

target_include_directories(TitleBar PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(TitleBar PUBLIC
    Qt::Widgets
    Qt::Core
    qlementine
    WidgetFrame
)
