@echo off

echo Building tests
cd test
if not exist ".\build" mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
