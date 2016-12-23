cmake_minimum_required (VERSION 2.8.8)
project (arduino_mock_sample)
find_package (Threads)

add_subdirectory(arduino_mock)
set(SMARTCAR_MOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/smartcar_mock)

include_directories(
    ${ARDUINO_MOCK_INCLUDE_DIRS}
    ${SMARTCAR_MOCK_DIR}
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/include
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock/include
)

file(GLOB SRC "DigitalReadSerial_unittest.cc")
add_executable(arduino_mock_sample DigitalReadSerial_unittest.cc)

target_link_libraries(arduino_mock_sample
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest-build/libgtest.a
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock-build/libgmock.a
    ${ARDUINO_MOCK_LIBS_DIR}/dist/lib/libarduino_mock.a
    ${CMAKE_THREAD_LIBS_INIT}
)

add_dependencies(arduino_mock_sample arduino_mock)

enable_testing()
add_test(arduino_mock_sample_1 arduino_mock_sample)
