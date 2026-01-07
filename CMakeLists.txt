cmake_minimum_required(VERSION 3.15)

file(STRINGS "version.txt" project_version)
string(REPLACE "." ";" version_list ${project_version})
list(GET version_list 0 VERSION_MAJOR)
list(GET version_list 1 VERSION_MINOR)
list(GET version_list 2 VERSION_PATCH)
list(LENGTH version_list version_length)
if(version_length GREATER 3)
    list(GET version_list 3 VERSION_TWEAK)
    set(project_version_cmake
        "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}"
    )
else()
    set(project_version_cmake
        "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
    )
endif()

project(qualpal LANGUAGES CXX VERSION ${project_version_cmake})

option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_CLI "Build CLI application" OFF)
option(BUILD_VISUALIZATIONS "Build visualization tools" OFF)
option(BUILD_WASM "Build WebAssembly bindings" OFF)
option(DAP_DEBUG "Interactive debugging" OFF)
option(ENABLE_COVERAGE "Generate coverage report" OFF)
option(ENABLE_CLI_TESTS "Enable CLI tests" ON)
option(ENABLE_SANITIZER "Enable address/undefined sanitizers" OFF)
option(ENABLE_OPENMP "Enable OpenMP parallelization" ON)

include(FetchContent)
include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(DAP_DEBUG)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wno-cpp")
endif()

if(ENABLE_SANITIZER)
    set(SANITIZER_FLAGS
        "-fsanitize=address,undefined -fno-omit-frame-pointer -g"
    )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZER_FLAGS}")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(MSVC)
    add_compile_definitions(_USE_MATH_DEFINES)
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
    COMPONENT Development
)
install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/qualpal-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qualpal
    COMPONENT Development
)

set(OpenMP_RUNTIME_MSVC "llvm")

if(ENABLE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_FOUND)
        message(STATUS "OpenMP found: ${OpenMP_CXX_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    endif()
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
    COMPONENT Development
    FILES_MATCHING
    PATTERN "*.h"
)

install(
    TARGETS qualpal
    EXPORT qualpal-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Libraries
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Libraries
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT qualpal-targets
    FILE qualpal-targets.cmake
    NAMESPACE qualpal::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qualpal
    COMPONENT Development
)

if(BUILD_CLI)
    find_package(CLI11 2.6 QUIET)
    if(NOT CLI11_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            CLI11
            GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
            GIT_TAG v2.6.1
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
    elseif(NOT WIN32)
        message(WARNING "help2man not found - man page will not be generated")
    endif()
endif()

if(BUILD_TESTING)
    find_package(Catch2 3.12 QUIET)
    if(NOT Catch2_FOUND)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.12.0
        )
        FetchContent_MakeAvailable(Catch2)
    endif()

    set(ADD_CLI_TESTS OFF)
    if(ENABLE_CLI_TESTS AND NOT WIN32)
        set(ADD_CLI_TESTS ON)
    endif()

    set(TEST_SOURCES
        tests/analyze.cpp
        tests/benchmarks.cpp
        tests/metrics.cpp
        tests/colors.cpp
        tests/color_difference.cpp
        tests/core.cpp
        tests/cvd.cpp
        tests/math.cpp
        tests/matrix.cpp
        tests/utils.cpp
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
        CONCAT AWESOME_CSS_URL
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
        docs/examples.md
        docs/mainpage.md
        docs/getting_started.md
        include/
        ALL
        COMMENT "Generate package documentation"
    )
endif()

if(BUILD_WASM)
    # Check if we're using Emscripten
    if(NOT EMSCRIPTEN)
        message(
            FATAL_ERROR
            "BUILD_WASM requires Emscripten toolchain. Use: emcmake cmake ..."
        )
    endif()

    # WebAssembly bindings executable
    add_executable(qualpal_wasm src/wasm/bindings.cpp)
    target_link_libraries(qualpal_wasm PRIVATE qualpal::qualpal)

    target_link_options(
        qualpal_wasm
        PRIVATE
            -O3
            -flto
            -sWASM=1
            -sEXPORT_ES6=1
            -sMODULARIZE=1
            -sEXPORT_NAME=createQualpalModule
            -sALLOW_MEMORY_GROWTH=1
            -sDISABLE_EXCEPTION_CATCHING=0
            --bind
    )

    set_target_properties(
        qualpal_wasm
        PROPERTIES
            OUTPUT_NAME "qualpal"
            SUFFIX ".mjs"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/wasm"
    )

    # Copy WASM files to web public directory for development
    add_custom_command(
        TARGET qualpal_wasm
        POST_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/web/src"
        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/wasm/qualpal.mjs"
            "${CMAKE_CURRENT_SOURCE_DIR}/web/src/qualpal.mjs"
        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/wasm/qualpal.wasm"
            "${CMAKE_CURRENT_SOURCE_DIR}/web/src/qualpal.wasm"
        COMMENT "Copying WASM files to web/src for development"
    )
endif()

# Uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE
        @ONLY
    )

    add_custom_target(
        uninstall
        COMMAND
            ${CMAKE_COMMAND} -P
            ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
endif()

install(FILES LICENSE DESTINATION ${CMAKE_INSTALL_DOCDIR})

# CPack configuration for packaging

# Basic package information
set(CPACK_PACKAGE_NAME "qualpal")
set(CPACK_PACKAGE_VENDOR "Johan Larsson")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Qualitative color palette generation library"
)
set(CPACK_PACKAGE_VERSION ${project_version})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "Johan Larsson <johan@jolars.co>")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/jolars/qualpal")

# Resource file
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

# Debian-specific settings
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Johan Larsson <johan@example.com>")
set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/jolars/qualpal")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
    "C++ library for generating qualitative color palettes with maximum perceptual distinctiveness for data visualization. It uses advanced color science algorithms to create palettes that are optimally distinguishable for human perception, including considerations for color vision deficiency."
)

# Component-based packaging for separate library and CLI packages
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL Libraries Development CLI)

# Library component
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "qualpal Runtime Library")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION
    "Runtime library for qualpal color palette generation"
)
set(CPACK_COMPONENT_LIBRARIES_GROUP "Runtime")

# Development component
set(CPACK_COMPONENT_DEVELOPMENT_DISPLAY_NAME "qualpal Development Files")
set(CPACK_COMPONENT_DEVELOPMENT_DESCRIPTION
    "Header files and CMake configuration for qualpal development"
)
set(CPACK_COMPONENT_DEVELOPMENT_GROUP "Development")
set(CPACK_COMPONENT_DEVELOPMENT_DEPENDS Libraries)

# CLI component
set(CPACK_COMPONENT_CLI_DISPLAY_NAME "qualpal CLI Tool")
set(CPACK_COMPONENT_CLI_DESCRIPTION
    "Command-line interface for qualpal color palette generation"
)
set(CPACK_COMPONENT_CLI_GROUP "Applications")
set(CPACK_COMPONENT_CLI_DEPENDS Libraries)

# Debian package names for each component
set(CPACK_DEBIAN_LIBRARIES_PACKAGE_NAME "libqualpal1")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_NAME "libqualpal-dev")
set(CPACK_DEBIAN_CLI_PACKAGE_NAME "qualpal")

# Debian dependencies for each component
set(CPACK_DEBIAN_LIBRARIES_PACKAGE_DEPENDS "libc6, libstdc++6, libgomp1")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_DEPENDS
    "libqualpal1 (= ${project_version})"
)
set(CPACK_DEBIAN_CLI_PACKAGE_DEPENDS
    "libqualpal1 (= ${project_version}), libc6, libstdc++6"
)

# Debian sections
set(CPACK_DEBIAN_LIBRARIES_PACKAGE_SECTION "libs")
set(CPACK_DEBIAN_DEVELOPMENT_PACKAGE_SECTION "libdevel")
set(CPACK_DEBIAN_CLI_PACKAGE_SECTION "utils")

# RPM-specific settings
set(CPACK_RPM_PACKAGE_SUMMARY "Qualitative color palette generation library")
set(CPACK_RPM_PACKAGE_DESCRIPTION
    "C++ library for generating qualitative color palettes with maximum perceptual distinctiveness for data visualization. It uses advanced color science algorithms to create palettes that are optimally distinguishable for human perception, including considerations for color vision deficiency."
)
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_URL "https://github.com/jolars/qualpal")
set(CPACK_RPM_PACKAGE_VENDOR "Johan Larsson")

# Component-based RPM packaging
set(CPACK_RPM_COMPONENT_INSTALL ON)

# RPM package names for each component
set(CPACK_RPM_LIBRARIES_PACKAGE_NAME "libqualpal1")
set(CPACK_RPM_DEVELOPMENT_PACKAGE_NAME "libqualpal-devel")
set(CPACK_RPM_CLI_PACKAGE_NAME "qualpal")

# RPM dependencies for each component
set(CPACK_RPM_LIBRARIES_PACKAGE_REQUIRES "glibc, libstdc++, libgomp")
set(CPACK_RPM_DEVELOPMENT_PACKAGE_REQUIRES "libqualpal1 = ${project_version}")
set(CPACK_RPM_CLI_PACKAGE_REQUIRES
    "libqualpal1 = ${project_version}, glibc, libstdc++"
)

# RPM groups for each component
set(CPACK_RPM_LIBRARIES_PACKAGE_GROUP "System Environment/Libraries")
set(CPACK_RPM_DEVELOPMENT_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_CLI_PACKAGE_GROUP "Applications/Engineering")

# RPM summaries for each component
set(CPACK_RPM_LIBRARIES_PACKAGE_SUMMARY "qualpal runtime library")
set(CPACK_RPM_DEVELOPMENT_PACKAGE_SUMMARY "qualpal development files")
set(CPACK_RPM_CLI_PACKAGE_SUMMARY "qualpal command-line tool")

# RPM descriptions for each component
set(CPACK_RPM_LIBRARIES_PACKAGE_DESCRIPTION
    "Runtime library for qualpal color palette generation"
)
set(CPACK_RPM_DEVELOPMENT_PACKAGE_DESCRIPTION
    "Header files and CMake configuration for qualpal development"
)
set(CPACK_RPM_CLI_PACKAGE_DESCRIPTION
    "Command-line interface for qualpal color palette generation"
)

# Include CPack after all configuration
include(CPack)
