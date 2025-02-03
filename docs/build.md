# Building

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
make -j$(nproc)
bin/FreePictureSplitter
````

Try it!

## Linux
First, you should make sure that you have installed the newest C++ compiler and wxWidgets.
Then, clone the source.
````
git clone https://github.com/zxunge/FreePictureSplitter.git
````
Enter the source directory, run
````
mkdir build && cd $_
cmake ..
make -j$(nproc)
bin/FreePictureSplitter
````

