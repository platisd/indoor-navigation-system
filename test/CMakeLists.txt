cmake_minimum_required (VERSION 2.8.8)
project (smartcar_test)
find_package (Threads)

add_subdirectory(arduino_mock)
set(SMARTCAR_MOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/smartcar_mock)

include_directories(
    ${ARDUINO_MOCK_INCLUDE_DIRS}
    ${SMARTCAR_MOCK_DIR}
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/include
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock/include
)

file(GLOB TESTS_SRC ${CMAKE_SOURCE_DIR}ut/*)

foreach( testsourcefile ${TESTS_SRC} )
    string( REPLACE ".cc" "" testname ${testsourcefile} )
    add_executable( ${testname} ${testsourcefile} )
    target_link_libraries(${testname}
        ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest-build/libgtest.a
        ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock-build/libgmock.a
        ${ARDUINO_MOCK_LIBS_DIR}/dist/lib/libarduino_mock.a
        ${CMAKE_THREAD_LIBS_INIT}
    )
    add_dependencies(${testname} arduino_mock)

    enable_testing()
    add_test(${testname} ${testname})
endforeach( testsourcefile ${TESTS_SRC} )
