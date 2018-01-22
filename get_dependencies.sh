#!/bin/bash

caller_dir=$PWD
# Move to the current script directory
cd "${0%/*}"

# Install SQLite3
sudo apt-get install sqlite3 libsqlite3-dev -y
# Install Pistache
git clone https://github.com/oktal/pistache.git
cd pistache
mkdir build && cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install

# Install cmake 3.10
sudo apt-get purge cmake
version=3.10
build=2
mkdir ~/temp && cd ~/temp
wget https://cmake.org/files/v$version/cmake-$version.$build.tar.gz
tar -xzvf cmake-$version.$build.tar.gz && cd cmake-$version.$build/
./bootstrap
make -j4
sudo make install

# Go back to the initial directory when you are done
cd $caller_dir
