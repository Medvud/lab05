cmake_minimum_required(VERSION 3.10)
project(project)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(banking)

if(BUILD_TESTS)
  add_subdirectory(third-party/gtest)
endif()