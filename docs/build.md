# Building

The 1.x and 2.x versions need wxWidgets, while the incoming 3.x and later versions need Qt 5+.

## Windows
> [!NOTE]
> We only support from MSYS2 terminal.

First, install necessary packages.
````
pacman -S mingw-w64-x86_64-toolchain                \
          mingw-w64-x86_64-wxwidgets3.2-msw         \
          mingw-w64-x86_64-wxwidgets3.2-common      \
          base-devel                                \
          git
````
Then, clone the source.
````
git clone https://github.com/zxunge/FreePictureSplitter.git
````
Enter the source directory, run
````
mkdir build && cd $_
cmake ..
cmake --build . --parallel $(nproc)
bin/FreePictureSplitter
````

Try it!

## UNIX/Linux
First, you should make sure that you have installed the newest C++ compiler and wxWidgets.
Then, clone the source.
````
git clone https://github.com/zxunge/FreePictureSplitter.git
````
Enter the source directory, run
````
mkdir build && cd $_
cmake ..
cmake --build . --parallel $(nproc)
bin/FreePictureSplitter
````
