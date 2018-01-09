cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(test_ins_server)

# CXX flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-sign-compare -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat=2 -g")

# Include lcov for coverage reports
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../CmakeModules)
include(CodeCoverage)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fprofile-arcs -ftest-coverage")

set(REPOSITORY_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../)

# Download and unpack googletest at configure time
configure_file(test_libs/CMakeLists.txt
googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
include_directories("${gtest_SOURCE_DIR}/include"
    "${gmock_SOURCE_DIR}/include")

add_definitions(-DENABLE_TESTS)
enable_testing()

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


    #mocks
    mocks/mock_data_store.hpp
    mocks/mock_data_store.cpp
    mocks/mock_localization.hpp
    mocks/mock_localization.cpp
    mocks/mock_lib_wrapper.hpp
    mocks/mock_lib_wrapper.cpp

    suite_ins_service.cpp
)
target_link_libraries(test_ins_service gtest gmock_main pistache.a sqlite3.a dl )

# test DataStore class
add_executable(test_data_store
    ${REPOSITORY_ROOT}/include/data_store.hpp
    ${REPOSITORY_ROOT}/src/data_store.cpp
    suite_data_store.cpp
)
target_link_libraries(test_data_store gtest gmock_main sqlite3.a dl )

# test Localization class
add_executable(test_localization
    ${REPOSITORY_ROOT}/include/localization.hpp
    ${REPOSITORY_ROOT}/src/localization.cpp
    suite_localization.cpp
)
target_link_libraries(test_localization  gtest gmock_main)

set(GTEST_RUN_FLAGS --gtest_color=yes CACHE STRING "Flags passed to GTest")
add_test(INS_SERVICE_TEST test_ins_service ${GTEST_RUN_FLAGS})
add_test(DATA_STORE_TEST test_data_store ${GTEST_RUN_FLAGS})
add_test(LOCALIZATION_TEST test_localization ${GTEST_RUN_FLAGS})

# Add coverage reports
setup_target_for_coverage(NAME INS_SERVICE_TEST_coverage EXECUTABLE test_ins_service DEPENDENCIES test_ins_service)
setup_target_for_coverage(NAME DATA_STORE_TEST_coverage EXECUTABLE test_data_store DEPENDENCIES test_data_store)
setup_target_for_coverage(NAME LOCALIZATION_TEST_coverage EXECUTABLE test_localization DEPENDENCIES test_localization)
