#!/bin/bash
set -eu
export GTEST_COLOR=1

cd test
cd -- "$(dirname -- "$0")"
arg=${1:-n}
if [ "$arg" = "--clean" ] || [ "$arg" = "-c" ]; then
    rm -rf build
fi
mkdir -p build
cd build
cmake ..
make
ctest -V
cd ..
