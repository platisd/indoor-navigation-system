cmake_minimum_required(VERSION 3.8)
project(ins_server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES main.cpp ins_service.cpp ins_service.hpp data_store.cpp data_store.hpp
    types.hpp localization.hpp localization.cpp)

include_directories(${CMAKE_FIND_ROOT_PATH}/include)
link_directories(${CMAKE_FIND_ROOT_PATH}/lib)
add_executable(ins_server ${SOURCE_FILES})
target_link_libraries(ins_server pistache.a sqlite3.a dl)