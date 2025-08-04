target_sources(FreePictureSplitter PRIVATE
    fpsaboutdialog.cpp
    fpsaboutdialog.h
    fpsbatchdialog.cpp
    fpsbatchdialog.h
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
    fpssettingsdialog.cpp
    fpssettingsdialog.h
)

target_include_directories(FreePictureSplitter PRIVATE ${CMAKE_CURRENT_LIST_DIR})
