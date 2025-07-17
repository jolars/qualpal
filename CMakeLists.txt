cmake_minimum_required(VERSION 3.15)

file(STRINGS "version.txt" project_version)

project(qualpal LANGUAGES CXX VERSION ${project_version})

option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_CLI "Build CLI application" OFF)
option(BUILD_VISUALIZATIONS "Build visualization tools" OFF)
option(DAP_DEBUG "Interactive debugging" OFF)
option(ENABLE_COVERAGE "Generate coverage report" OFF)
option(ENABLE_CLI_TESTS "Enable CLI tests" ON)

include(FetchContent)
include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(DAP_DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wno-cpp")
endif()

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/qualpal-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/qualpal-config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qualpal
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/qualpal-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/qualpal-config.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qualpal
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/qualpal-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qualpal
)

find_package(OpenMP)

if(OpenMP_FOUND)
    message(STATUS "OpenMP found: ${OpenMP_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_subdirectory(src)

target_include_directories(
    qualpal
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(qualpal PUBLIC cxx_std_17)
add_library(qualpal::qualpal ALIAS qualpal)
set_target_properties(
    qualpal
    PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/qualpal/
    DESTINATION include/qualpal
    FILES_MATCHING
    PATTERN "*.h"
)

install(
    TARGETS qualpal
    EXPORT qualpal-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT qualpal-targets
    FILE qualpal-targets.cmake
    NAMESPACE qualpal::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qualpal
)

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
    target_link_libraries(qualpal_cli PRIVATE qualpal::qualpal CLI11::CLI11)
    target_include_directories(qualpal_cli PRIVATE "${PROJECT_BINARY_DIR}")

    install(TARGETS qualpal_cli DESTINATION bin COMPONENT CLI)

    find_program(HELP2MAN_EXECUTABLE help2man)
    if(HELP2MAN_EXECUTABLE)
        unset(SOURCE_DATE_EPOCH)
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

    set(ADD_CLI_TESTS OFF)
    if(ENABLE_CLI_TESTS AND NOT WIN32)
        set(ADD_CLI_TESTS ON)
    endif()

    set(TEST_SOURCES
        tests/benchmarks.cpp
        tests/metrics.cpp
        tests/colors.cpp
        tests/color_difference.cpp
        tests/core.cpp
        tests/cvd.cpp
        tests/math.cpp
        tests/matrix.cpp
        tests/validation.cpp
    )
    if(ADD_CLI_TESTS)
        list(APPEND TEST_SOURCES tests/cli.cpp)
    endif()

    add_executable(tests ${TEST_SOURCES})
    target_link_libraries(tests PRIVATE Catch2::Catch2WithMain qualpal::qualpal)
    target_include_directories(
        tests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            tests
            "${PROJECT_BINARY_DIR}"
    )

    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    include(Catch)
    catch_discover_tests(tests)

    if(ENABLE_COVERAGE)
        find_package(codecov)
        add_coverage(tests)
        add_coverage(qualpal)
        if(BUILD_CLI)
            add_coverage(qualpal_cli)
        endif()
        coverage_evaluate(tests-gcov)
    endif()
endif()

if(BUILD_VISUALIZATIONS)
    # Check for gnuplot
    find_package(Gnuplot REQUIRED)

    # Data generation executable
    add_executable(generate_data src/visualization/generate_data.cpp)
    target_link_libraries(generate_data PRIVATE qualpal::qualpal)

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
    set(DOXYGEN_EXCLUDE_SYMBOLS qualpal::metrics::detail qualpal::detail)
    set(DOXYGEN_EXTRACT_ALL NO)
    set(DOXYGEN_EXTRACT_PRIVATE NO)
    set(DOXYGEN_EXTRACT_STATIC NO)
    set(DOXYGEN_FULL_SIDEBAR NO)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_HTML_COLORSTYLE LIGHT)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(DOXYGEN_PROJECT_LOGO "${CMAKE_CURRENT_SOURCE_DIR}/docs/images/logo.svg")
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_USE_MATHJAX YES)
    set(DOXYGEN_WARN_AS_ERROR YES)
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)

    doxygen_add_docs(
        docs
        docs/algorithm.md
        docs/colors.md
        docs/mainpage.md
        docs/getting_started.md
        include/
        ALL
        COMMENT "Generate package documentation"
    )
endif()

install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR})
