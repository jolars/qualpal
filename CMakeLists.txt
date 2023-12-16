cmake_minimum_required(VERSION 3.15)

file(STRINGS "version.txt" project_version)

project(
  qualpal
  LANGUAGES CXX
  VERSION ${project_version})

option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_R_BINDINGS "Build R bindings" OFF)
option(BUILD_CLI "Build CLI application" ON)
option(ENABLE_COVERAGE "Generate coverage report" OFF)

include(FetchContent)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(PROJECT_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)

add_subdirectory(bindings)

if(BUILD_CLI)
  add_executable(qualpal_cli src/main.cpp)
  set_property(TARGET qualpal_cli PROPERTY OUTPUT_NAME qualpal)

  FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2)

  FetchContent_MakeAvailable(cli11)

  target_link_libraries(qualpal_cli PUBLIC qualpal CLI11::CLI11)
  target_include_directories(qualpal_cli PUBLIC "${PROJECT_BINARY_DIR}")
endif()

if(BUILD_TESTING)
  find_package(Catch2 3 QUIET)
  if(NOT Catch2_FOUND)
    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG v3.4.0)
    FetchContent_MakeAvailable(Catch2)
  endif()

  add_executable(tests tests/test.cpp)
  target_link_libraries(tests PRIVATE Catch2::Catch2WithMain qualpal)
  target_include_directories(tests PUBLIC tests "${PROJECT_BINARY_DIR}")

  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
  include(Catch)
  catch_discover_tests(tests)

  if(ENABLE_COVERAGE)
    find_package(codecov)
    add_coverage(tests)
    add_coverage(qualpal)
    coverage_evaluate(tests-gcov)
  endif()
endif()

if(BUILD_DOCS)
  find_package(Doxygen REQUIRED)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
  doxygen_add_docs(docs ${PROJECT_SOURCE_DIR} docs/ ALL
                   COMMENT "Generate package documentation")
endif()
