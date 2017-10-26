cmake_minimum_required (VERSION 2.8.8)
project (INS_Node_Test_Suite)
find_package (Threads)

add_subdirectory(arduino_mock)
set(INS_MOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ins_mock)
set(UT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ut)

enable_testing()

include_directories(
    ${ARDUINO_MOCK_INCLUDE_DIRS}
    ${INS_MOCK_DIR}
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/include
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock/include
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
    add_test(${testname} ${testname})
endforeach(test ${TESTS})
