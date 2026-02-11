target_sources(FreePictureSplitter PRIVATE
    aboutdialog.cpp
    aboutdialog.h
    draggableline.cpp
    draggableline.h
    graphicsview.cpp
    graphicsview.h
    mainwindow.cpp
    mainwindow.h
    progressdialog.cpp
    progressdialog.h
    rulerbar.cpp
    rulerbar.h
    clickablelabel.cpp
    clickablelabel.h
    singlewidget.cpp
    singlewidget.h
    batchwidget.cpp
    batchwidget.h
    preferenceswidget.cpp
    preferenceswidget.h
    fancytabwidget.cpp
    fancytabwidget.h
    errorlogdialog.cpp
    errorlogdialog.h
)

target_include_directories(FreePictureSplitter PRIVATE ${CMAKE_CURRENT_LIST_DIR})
