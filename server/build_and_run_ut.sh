#!/bin/bash
caller_dir=$PWD
# Move to the current script directory
cd "${0%/*}"

set -eu
export GTEST_COLOR=1

cd test
arg=${1:-n}
if [ "$arg" = "--clean" ] || [ "$arg" = "-c" ]; then
    rm -rf build
fi
mkdir -p build
cd build
cmake ..
make
ctest -V

# Go back to the initial directory when you are done
cd $caller_dir