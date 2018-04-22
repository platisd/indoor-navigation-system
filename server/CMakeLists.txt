cmake_minimum_required(VERSION 3.2)
project(ins_server)

set(CMAKE_C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package (LibXml2)

set(SOURCE_FILES
    src/main.cpp
    src/ins_service.cpp
    src/data_store.cpp
    src/localization.cpp
    src/lib_wrapper.cpp
    src/WifiNode.c
    src/WifiAccessPointLocalConfig.c
 )

include_directories(
    ${CMAKE_FIND_ROOT_PATH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/external
    ${LIBXML2_INCLUDE_DIR})


add_executable(ins_server ${SOURCE_FILES})
target_link_libraries(ins_server pistache.a sqlite3.a dl m ${LIBXML2_LIBRARIES})
