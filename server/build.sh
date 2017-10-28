#!/bin/bash

caller_dir=$PWD
# Move to the current script directory
cd "${0%/*}"

# Build the component
rm -rf build
mkdir build
cd build
cmake ..
make
# Go back to the initial directory when you are done
cd $caller_dir
