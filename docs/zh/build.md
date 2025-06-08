# 从源码构建FreePictureSplitter

> [!NOTE]
> 本软件的1.x和2.x版本使用了wxWidgets 3库，而3.x及以上版本使用Qt库。
> 因此，目前的main分支和版本3及以上的源码tar归档文件所包含的文档均只提供构建Qt GUI的说明。

## Windows

> [!NOTE]
> 我们只支持从MSYS2终端中构建。

首先安装工具链和Qt （你还需要Git以便从GitHub克隆项目）：
````
pacman -S mingw-w64-x86_64-toolchain  \
          mingw-w64-x86_64-cmake      \
          mingw-w64-x86_64-ninja      \
          mingw-w64-x86_64-qt6-base   \
          mingw-w64-x86_64-qt6-tools  \
          git
````
Qt5同样受到支持：
````
pacman -S mingw-w64-x86_64-toolchain  \
          mingw-w64-x86_64-cmake      \
          mingw-w64-x86_64-ninja      \
          mingw-w64-x86_64-qt5-base   \
          mingw-w64-x86_64-qt5-tools  \
          git
````
进入项目源码目录，运行
````
git submodule update --init --recursive
mkdir build && cd $_
cmake ..
cmake --build . --parallel $(nproc)
./FreePictureSplitter
````
享受它吧！

## Linux发行版
在不同的Linux发行版上，安装包的方法可能有所不同。然而，“尝一脔肉，而知一镬之味”，我们介绍一下用apt安装相关包的过程。
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
或
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
其余与上述Msys2操作相同。