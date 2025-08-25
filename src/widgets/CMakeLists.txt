target_sources(FreePictureSplitter PRIVATE
    fpsaboutdialog.cpp
    fpsaboutdialog.h
    fpsdraggableline.cpp
    fpsdraggableline.h
    fpsgraphicsview.cpp
    fpsgraphicsview.h
    fpsmainwindow.cpp
    fpsmainwindow.h
    fpsprogressdialog.cpp
    fpsprogressdialog.h
    fpsrulerbar.cpp
    fpsrulerbar.h
    fpsclickablelabel.cpp
    fpsclickablelabel.h
    fpssinglewidget.cpp
    fpssinglewidget.h
    fpsbatchwidget.cpp
    fpsbatchwidget.h
    fpspreferenceswidget.cpp
    fpspreferenceswidget.h
)

target_include_directories(FreePictureSplitter PRIVATE ${CMAKE_CURRENT_LIST_DIR})
