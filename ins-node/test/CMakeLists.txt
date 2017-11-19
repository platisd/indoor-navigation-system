cmake_minimum_required (VERSION 2.8.8)
project (INS_Node_Test_Suite)
find_package (Threads)

add_subdirectory(arduino_mock)
set(INS_MOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ins_mock)
set(STUBS ${CMAKE_CURRENT_SOURCE_DIR}/stubs)
set(UT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ut)
set(GTEST_RUN_FLAGS --gtest_color=yes --gtest_repeat=5 --gtest_shuffle)

#Include lcov for coverage reports
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../CmakeModules)
include(CodeCoverage)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")

enable_testing()

include_directories(
    ${ARDUINO_MOCK_INCLUDE_DIRS}
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/include
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock/include
    ${INS_MOCK_DIR}
    ${STUBS}
)

# Include all files in ut/ ending with .cc as tests
file(GLOB TESTS ${UT_DIR}/*.cc)
# Iterate through the files inside the unit tests folder
foreach(test ${TESTS})
    # Print the included path
    MESSAGE(STATUS "Included test file: " ${test})
    # Remove the file ending and the path, leaving just the file name
    string(REPLACE ".cc" "" testname ${test})
    string(REPLACE ${UT_DIR}/ "" testname ${testname})
    # Add the test source file to an executable with the same name
    add_executable(${testname} ${test})
    target_link_libraries(
        ${testname}
        ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest-build/libgtest.a
        ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock-build/libgmock.a
        ${ARDUINO_MOCK_LIBS_DIR}/dist/lib/libarduino_mock.a
        ${CMAKE_THREAD_LIBS_INIT}
    )
    add_dependencies(${testname} arduino_mock)
    # Add it as a separate test
    add_test(${testname} ${testname} ${GTEST_RUN_FLAGS})
    # Add coverage report
    setup_target_for_coverage(NAME ${testname}_coverage EXECUTABLE ${testname} DEPENDENCIES ${testname})
endforeach(test ${TESTS})
