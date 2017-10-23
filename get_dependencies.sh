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
# Go back to the initial directory when you are done
cd $caller_dir
