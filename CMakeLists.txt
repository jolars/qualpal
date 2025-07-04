cmake_minimum_required(VERSION 3.15)

file(STRINGS "version.txt" project_version)

project(qualpal LANGUAGES CXX VERSION ${project_version})

option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_CLI "Build CLI application" OFF)
option(BUILD_VISUALIZATIONS "Build visualization tools" OFF)
option(ENABLE_COVERAGE "Generate coverage report" OFF)

include(FetchContent)
include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/qualpal/
    DESTINATION include/qualpal
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "qualpal"
)

target_include_directories(
    qualpal
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS qualpal DESTINATION lib)

if(BUILD_CLI)
    find_package(CLI11 2.4 QUIET)
    if(NOT CLI11_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            CLI11
            GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
            GIT_TAG v2.4.2
            GIT_SHALLOW TRUE
        )
        FetchContent_MakeAvailable(CLI11)
    endif()

    add_executable(qualpal_cli src/main.cpp)
    set_property(TARGET qualpal_cli PROPERTY OUTPUT_NAME qualpal)
    target_link_libraries(qualpal_cli PUBLIC qualpal CLI11::CLI11)
    target_include_directories(qualpal_cli PUBLIC "${PROJECT_BINARY_DIR}")

    install(TARGETS qualpal_cli DESTINATION bin COMPONENT CLI)

    find_program(HELP2MAN_EXECUTABLE help2man)
    if(HELP2MAN_EXECUTABLE)
        unset(SOURCE_DATE_EPOCH)
        message(STATUS "Using SOURCE_DATE_EPOCH: ${SOURCE_DATE_EPOCH}")
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/qualpal.1
            COMMAND
                ${CMAKE_COMMAND} -E env "SOURCE_DATE_EPOCH=${SOURCE_DATE_EPOCH}"
                ${HELP2MAN_EXECUTABLE}
                --name="automatic generation of qualitative color palettes"
                --section=1 --no-info --version-string=${PROJECT_VERSION}
                --help-option="--help"
                --include=${CMAKE_CURRENT_SOURCE_DIR}/docs/qualpal.help2man
                --output=${CMAKE_CURRENT_BINARY_DIR}/qualpal.1
                $<TARGET_FILE:qualpal_cli>
            DEPENDS
                qualpal_cli
                ${CMAKE_CURRENT_SOURCE_DIR}/docs/qualpal.help2man
            COMMENT "Generating man page with help2man"
        )

        add_custom_target(man ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/qualpal.1)

        install(
            FILES ${CMAKE_CURRENT_BINARY_DIR}/qualpal.1
            DESTINATION share/man/man1
            COMPONENT CLI
        )
    else()
        message(WARNING "help2man not found - man page will not be generated")
    endif()
endif()

if(BUILD_TESTING)
    find_package(Catch2 3.6 QUIET)
    if(NOT Catch2_FOUND)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.6.0
        )
        FetchContent_MakeAvailable(Catch2)
    endif()

    add_executable(
        tests
        tests/colors.cpp
        tests/core.cpp
        tests/cvd.cpp
        tests/math.cpp
        tests/validation.cpp
    )
    target_link_libraries(tests PRIVATE Catch2::Catch2WithMain qualpal)
    target_include_directories(tests PUBLIC tests "${PROJECT_BINARY_DIR}")

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(tests)

    if(ENABLE_COVERAGE)
        find_package(codecov)
        add_coverage(tests)
        add_coverage(qualpal)
        add_coverage(qualpal_cli)
        coverage_evaluate(tests-gcov)
    endif()
endif()

if(BUILD_VISUALIZATIONS)
    # Check for gnuplot
    find_package(Gnuplot REQUIRED)

    # Data generation executable
    add_executable(generate_data src/visualization/generate_data.cpp)
    target_link_libraries(generate_data PRIVATE qualpal)

    add_custom_target(
        plots
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/docs/data
        COMMAND
            ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/docs/images
        COMMAND $<TARGET_FILE:generate_data>
        COMMAND chmod +x ${CMAKE_SOURCE_DIR}/docs/generate_plots.sh
        COMMAND ${CMAKE_SOURCE_DIR}/docs/generate_plots.sh
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        DEPENDS generate_data
        COMMENT "Generating documentation plots with gnuplot"
    )
endif()

if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)

    string(
        CONCAT
        AWESOME_CSS_URL
        "https://github.com/jothepro/"
        "doxygen-awesome-css/archive/refs/heads/main.zip"
    )

    FetchContent_Declare(
        doxygen-awesome-css
        URL ${AWESOME_CSS_URL}
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(doxygen-awesome-css)
    FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR AWESOME_CSS_DIR)

    # Generate the Doxyfile
    set(DOXYGEN_IMAGE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/docs/images")
    set(DOXYGEN_HTML_EXTRA_STYLESHEET
        "${AWESOME_CSS_DIR}/doxygen-awesome.css"
        "${AWESOME_CSS_DIR}/doxygen-awesome-sidebar-only.css"
    )
    set(DOXYGEN_DISABLE_INDEX NO)
    set(DOXYGEN_FULL_SIDEBAR NO)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HTML_COLORSTYLE LIGHT)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(DOXYGEN_PROJECT_LOGO "${CMAKE_CURRENT_SOURCE_DIR}/docs/images/logo.svg")
    set(DOXYGEN_USE_MATHJAX YES)
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_EXTRACT_ALL NO)
    set(DOXYGEN_EXTRACT_PRIVATE NO)
    set(DOXYGEN_EXTRACT_STATIC NO)

    doxygen_add_docs(
        docs
        docs/mainpage.md
        docs/algorithm.md
        include/
        ALL
        COMMENT "Generate package documentation"
    )
endif()
