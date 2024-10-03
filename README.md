# Introduction
FreePictureSplitter is a fast, small, and easy-to-use picture splitter!

## Goals 
FreePictureSplitter has the following goals:
Goals | Implementation
------|------
Batch splitting | 1.x only; working in 2.x
Multiple picture formats | √ 
Splitting by columns and rows | √ 
Single picture splitting | 2.x only
Splitting by pixels | 2.x only
User configurations | ×
HTML generation | ×
Quality control | ×

## Building
> [!NOTE]
> We only tested it on Windows. On UNIX/Linux, the following commands may work, too.
### Windows
> [!NOTE]
> We only support from MSYS2 terminal.

First, install necessary packages.
````
pacman -S mingw-w64-clang-x86_64-toolchain                \
          mingw-w64-clang-x86_64-wxwidgets3.2-msw         \
          mingw-w64-clang-x86_64-wxwidgets3.2-common      \
          base-devel                                      \
          git
````
Then, clone the source.
````
git clone https://github.com/zxunge/FreePictureSplitter.git
````
Enter the source directory, run
````
mkdir build && cd $_
cmake .. -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang
make -j$(nproc)
bin/FreePictureSplitter
````
> [!NOTE]
> We only support building using clang, as there will be puzzling errors using gcc.

Try it!
