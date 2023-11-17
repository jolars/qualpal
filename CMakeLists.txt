cmake_minimum_required(VERSION 3.15)

include(FetchContent)

project(
  qualpal
  LANGUAGES CXX
  VERSION 0.4.4.9000)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_R_BINDINGS "Build R bindings" OFF)

add_subdirectory(src)

add_subdirectory(bindings)

add_executable(qualpal src/main.cpp)

FetchContent_Declare(
  cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG v2.3.2)

FetchContent_MakeAvailable(cli11)

target_link_libraries(qualpal PUBLIC qualpal_lib CLI11::CLI11)

target_include_directories(qualpal PUBLIC "${PROJECT_BINARY_DIR}")

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0)
FetchContent_MakeAvailable(Catch2)

add_executable(tests tests/test.cpp)

target_include_directories(tests PUBLIC tests "${PROJECT_BINARY_DIR}")
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain qualpal_lib)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)
include(Catch)

catch_discover_tests(tests)
