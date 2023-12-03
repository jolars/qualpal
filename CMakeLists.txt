cmake_minimum_required(VERSION 3.15)

file(STRINGS "version.txt" project_version)

project(
  qualpal
  LANGUAGES CXX
  VERSION ${project_version})

option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_R_BINDINGS "Build R bindings" OFF)
option(BUILD_CLI "Build CLI application" ON)

include(FetchContent)
include(CTest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(PROJECT_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)

add_subdirectory(bindings)

if(BUILD_CLI)
  add_executable(qualpal src/main.cpp)

  FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2)

  FetchContent_MakeAvailable(cli11)

  target_link_libraries(qualpal PUBLIC qualpal_lib CLI11::CLI11)
  target_include_directories(qualpal PUBLIC "${PROJECT_BINARY_DIR}")
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
  target_link_libraries(tests PRIVATE Catch2::Catch2WithMain qualpal_lib)
  target_include_directories(tests PUBLIC tests "${PROJECT_BINARY_DIR}")

  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
  include(Catch)
  catch_discover_tests(tests)
endif()

if(BUILD_DOCS)
  find_package(Doxygen REQUIRED)
  set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_SOURCE_DIR}/docs/Doxyfile)
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  add_custom_target(
    docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    COMMENT "Generating Doxygen XML"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs/
    VERBATIM)
  add_custom_target(
    docs_sphinx ALL
    COMMAND make html
    COMMENT "Generating sphinx HTML"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs/
    VERBATIM)
endif()
