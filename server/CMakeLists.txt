cmake_minimum_required(VERSION 3.2)
project(ins_server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES
    src/main.cpp
    src/ins_service.cpp
    src/data_store.cpp
    src/localization.cpp
 )

include_directories(
    ${CMAKE_FIND_ROOT_PATH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include )

link_directories(${CMAKE_FIND_ROOT_PATH}/lib)
add_executable(ins_server ${SOURCE_FILES})
target_link_libraries(ins_server pistache.a sqlite3.a dl)
