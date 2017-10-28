cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(test_ins_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat=2 -g")

set(REPOSITORY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../)

add_subdirectory(test_libs)

add_definitions(-DENABLE_TESTS)
enable_testing()

#include(cmake/shared.cmake)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/test_libs/gmock-gtest
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/
    ${REPOSITORY_ROOT}
    ${REPOSITORY_ROOT}/include
    ${REPOSITORY_ROOT}/include/external
    ${REPOSITORY_ROOT}/src
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_FIND_ROOT_PATH}/include
)

link_directories(${CMAKE_FIND_ROOT_PATH}/lib)

# test IndoorNavigationService class
add_executable(test_ins_service
    ${REPOSITORY_ROOT}/include/ins_service.hpp
    ${REPOSITORY_ROOT}/src/ins_service.cpp
    ${REPOSITORY_ROOT}/src/data_store.cpp  # TODO - replace with mock
    ${REPOSITORY_ROOT}/src/localization.cpp # TODO - replace  with mock
    suite_ins_service.cpp
)
target_link_libraries(test_ins_service googletest pistache.a sqlite3.a dl )

# test DataStore class
add_executable(test_data_store
    ${REPOSITORY_ROOT}/include/data_store.hpp
    ${REPOSITORY_ROOT}/src/data_store.cpp
    suite_data_store.cpp
)
target_link_libraries(test_data_store googletest sqlite3.a dl )

# test Localization class
add_executable(test_localization
    ${REPOSITORY_ROOT}/include/localization.hpp
    ${REPOSITORY_ROOT}/src/localization.cpp
    suite_localization.cpp
)
target_link_libraries(test_localization googletest)

set(GTEST_RUN_FLAGS --gtest_color=yes --gtest_repeat=20 --gtest_shuffle CACHE STRING "Flags passed to GTest")
add_test(INS_SERVICE_TEST test_ins_service ${GTEST_RUN_FLAGS})
add_test(DATA_STORE_TEST test_data_store ${GTEST_RUN_FLAGS})
add_test(LOCALIZATION_TEST test_localization ${GTEST_RUN_FLAGS})
