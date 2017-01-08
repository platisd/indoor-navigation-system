#!/bin/sh
set -eu
export GTEST_COLOR=1

cd -- "$(dirname -- "$0")"
mkdir -p build
cd build
cmake ..
make
ctest -V
