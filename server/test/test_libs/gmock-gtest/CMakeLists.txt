cmake_minimum_required(VERSION 2.8)

project(googletest)

#include(../../cmake/shared.cmake)

add_library(${PROJECT_NAME} STATIC
   gmock/gmock.h
   gtest/gtest.h
   gmock-gtest-all.cc
   gmock-gtest-main.cc
  )

target_include_directories(${PROJECT_NAME}
   PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  )

target_link_libraries(${PROJECT_NAME}
   pthread
  )

