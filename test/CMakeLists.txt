cmake_minimum_required (VERSION 2.8.8)
project (Smartcar_Test_Suite)
find_package (Threads)

add_subdirectory(arduino_mock)
set(SMARTCAR_MOCK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/smartcar_mock)
set(UT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ut)

enable_testing()

include_directories(
    ${ARDUINO_MOCK_INCLUDE_DIRS}
    ${SMARTCAR_MOCK_DIR}
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest/include
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock/include
)

# file(TESTS_SRC ut/manualControl_test.cc)
add_executable(manualControlTest ${UT_DIR}/manualControl_test.cc)
target_link_libraries(manualControlTest
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest-build/libgtest.a
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock-build/libgmock.a
    ${ARDUINO_MOCK_LIBS_DIR}/dist/lib/libarduino_mock.a
    ${CMAKE_THREAD_LIBS_INIT}
)
add_dependencies(manualControlTest arduino_mock)


add_executable(digitalReadTest ${UT_DIR}/DigitalReadSerial_test.cc)
target_link_libraries(digitalReadTest
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gtest/gtest/src/gtest-build/libgtest.a
    ${ARDUINO_MOCK_LIBS_DIR}/lib/gmock/gmock/src/gmock-build/libgmock.a
    ${ARDUINO_MOCK_LIBS_DIR}/dist/lib/libarduino_mock.a
    ${CMAKE_THREAD_LIBS_INIT}
)
add_dependencies(digitalReadTest arduino_mock)


add_test(manual_control_test manualControlTest)
add_test(digital_read_test digitalReadTest)
