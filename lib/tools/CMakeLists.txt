qt_add_library(FramelessWidget STATIC
    framelesswidget.cpp
    framelesswidget.h
    titlebar.cpp
    titlebar.h
)

target_include_directories(FramelessWidget PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(FramelessWidget PUBLIC
    Qt::Widgets
    Qt::Core
)