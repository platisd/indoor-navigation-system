#!/bin/bash

caller_dir=$PWD
# Move to the current script directory
cd "${0%/*}"

# Build the component
rm -rf build
mkdir build
cd build
cmake -DCMAKE_CXX_CLANG_TIDY:STRING="clang-tidy;-checks=*;-warnings-as-errors=*,-*-pointer-arithmetic" .. || exit 1 
cmake --build . || exit 1
# Go back to the initial directory when you are done
cd $caller_dir
