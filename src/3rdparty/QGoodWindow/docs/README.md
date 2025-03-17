# QGoodWindow

**QGoodWindow** aims to provide a simple and ready-to-use way of implement a border less window, while **QGoodCentralWidget** aims to provide a widget with title bar and easy customization of itself aspects.

## Building:

To get the required build packages on Ubuntu based OS for both Qt 5 and Qt 6, use:

```
sudo apt install build-essential libgl1-mesa-dev
```

Also for development with Qt 5:

```
sudo apt install libgtk2.0-dev
```

Also for development with Qt 6:

```
sudo apt install libgtk-3-dev
```

Or install both if you want to develop with Qt 5 and Qt 6 on the same machine.

To build and use **QGoodWindow** with **QGoodCentralWidget** or not, you have three options: 

- Include the sources:
  
  - *CMake:*
    
    For use **QGoodWindow** add to your `CMakeLists.txt`:
    
    ```
    include(..path_to/QGoodWindow/QGoodWindow.cmake)
    ```
    
    For use **QGoodCentralWidget** add to your `CMakeLists.txt` after **QGoodWindow**:
    
    ```
    include(..path_to/QGoodCentralWidget/QGoodCentralWidget.cmake)
    ```
  
  - *QMake:*
    
    For use **QGoodWindow** add to your `.pro`:
    
    ```
    include(..path_to/QGoodWindow/QGoodWindow.pri)
    ```
    
    For use **QGoodCentralWidget** add to your `.pro` after **QGoodWindow**:
    
    ```
    include(..path_to/QGoodCentralWidget/QGoodCentralWidget.pri)
    ```

- Build as static library:
  
  - *CMake:*
    
    Build with *CMake* using the `CMakeLists.txt` found on `build-library` folder, by default it produces a static library, the *CMake GUI* interface can also be used.
    
    The *CMake* version allows to compile `QGoodWindow` with `QGoodCentralWidget` or `QGoodWindow` only.
  
  - *QMake:*
    
    Build with *QMake* by selecting one of the static `QMake` build options in the `build-library` folder, with or without `QGoodCentralWidget`.

- Build as shared library:
  
  - *CMake:*
    
    Build with *CMake* using the `CMakeLists.txt` found on `build-library` folder, by default it produces a static library, but you can edit the file by remove the commentary of `set(shared_library TRUE)` or use the *CMake GUI* interface with the option `shared_library` set.
    
    The *CMake* version allows to compile `QGoodWindow` with `QGoodCentralWidget` or `QGoodWindow` only.
  
  - *QMake:*
    
    Build with *QMake* by selecting one of the shared *QMake* build options in the `build-library` folder, with or without `QGoodCentralWidget`.

## Usage:

- To use **QGoodWindow** in your project add `#include <QGoodWindow>` in your code, call the static function `QGoodWindow::setup();` before `QApplication` instantiation, then use `QGoodWindow` as `QMainWindow`, for example, create an instance of `QGoodWindow` or sub class it.

- To use `QGoodCentralWidget` add `#include <QGoodCentralWidget>` in your code, set it as central widget of `QGoodWindow` and set to the `QGoodCentralWidget` it's own central widget.

## Examples:

To start developing with `QGoodWindow` you can create a simple window with:

```
#include <QGoodWindow>

int main(int argc, char *argv[])
{
    QGoodWindow::setup();

    QApplication app(argc, argv);

    QGoodWindow gw;
    gw.show();

    return app.exec();
}
```

To start developing with `QGoodWindow` and `QGoodCentralWidget` you can create a simple window with title bar with:

```
#include <QGoodWindow>
#include <QGoodCentralWidget>

int main(int argc, char *argv[])
{
    QGoodWindow::setup();

    QApplication app(argc, argv);

    QGoodWindow gw;
    QGoodCentralWidget gcw(&gw);
    gw.setCentralWidget(&gcw);
    gw.show();

    return app.exec();
}
```

To create a `QDialog` based modal window, you could start with a `QMessageBox`:

```
#include <QGoodWindow>
#include <QGoodCentralWidget>

int main(int argc, char *argv[])
{
    QGoodWindow::setup();

    QApplication app(argc, argv);

    QGoodWindow gw;
    QGoodCentralWidget gcw(&gw);
    gw.setCentralWidget(&gcw);
    gw.show();

    QMessageBox msgbox(&gw);
    msgbox.setIcon(QMessageBox::Information);
    msgbox.setText("Information");
    QGoodCentralWidget::execDialogWithWindow(&msgbox, &gw, &gcw);

    return app.exec();
}
```

## API:

- See the API list and description of `QGoodWindow` [here](QGoodWindow.md).
- See the API list and description of `QGoodCentralWidget` [here](QGoodCentralWidget.md).
