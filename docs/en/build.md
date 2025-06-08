# Building FreePictureSplitter from Source

> [!NOTE] 
> The 1.x and 2.x versions need wxWidgets, while the 3.x and later versions need Qt 5+.
> The build docs included in the main branch and the tarballs(version >= 3.0.0) are for building with Qt specifically.

## Windows
> [!NOTE]
> We only support building from the MSYS2 terminal.

First, install necessary packages for build toolchains and Qt(Git is needed if you want to clone the source from GitHub).
````
pacman -S mingw-w64-x86_64-toolchain  \
          mingw-w64-x86_64-cmake      \
          mingw-w64-x86_64-ninja      \
          mingw-w64-x86_64-qt6-base   \
          mingw-w64-x86_64-qt6-tools  \
          git
````
Qt5 is also supported for older computers, so the above commands are:
````
pacman -S mingw-w64-x86_64-toolchain  \
          mingw-w64-x86_64-cmake      \
          mingw-w64-x86_64-ninja      \
          mingw-w64-x86_64-qt5-base   \
          mingw-w64-x86_64-qt5-tools  \
          git
````
Then, clone the source.
````
git clone https://github.com/zxunge/FreePictureSplitter.git
````
Enter the source directory, run
````
git submodule update --init --recursive
mkdir build && cd $_
cmake ..
cmake --build . --parallel $(nproc)
./FreePictureSplitter
````

Enjoy it!

## UNIX/Linux
First, you should make sure that you have installed the newest C++ compiler and Qt by:
````
sudo apt update
sudo apt install -y          \
         build-essential     \
         qtbase5-dev         \
         qtbase5-private-dev \
         qttools5-dev-tools  \
         qttools5-dev        \
         libxkbcommon-dev
````
or
````
sudo apt update
sudo apt install -y           \
         build-essential      \
         qt6-base-dev         \
         qt6-base-private-dev \
         linguist-qt6         \
         qt6-tools-dev        \
         libxkbcommon-dev
````
Then, clone the source.
````
git clone https://github.com/zxunge/FreePictureSplitter.git
````
Enter the source directory, run
````
git submodule update --init --recursive
mkdir build && cd $_
cmake ..
cmake --build . --parallel $(nproc)
./FreePictureSplitter
````
Enjoy it!
