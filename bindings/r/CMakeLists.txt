find_package(R 4.0)

include(CMakePrintHelpers)

set(R_PKG_NAME qualpalr)
set(R_PKG_VERSION ${CMAKE_PROJECT_VERSION})
string(TIMESTAMP R_PKG_DATE "%Y-%m-%d")

# Copy all R package files to the build directory
set(R_PKG_IN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(R_PKG_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${R_PKG_NAME})
set(CPP_SRC_DIR "${CMAKE_SOURCE_DIR}/src/${CMAKE_PROJECT_NAME}")

# Create folder and copy files that won't change at configuration time
file(MAKE_DIRECTORY ${R_PKG_OUT_DIR}/src)
file(MAKE_DIRECTORY ${R_PKG_OUT_DIR}/src/${CMAKE_PROJECT_NAME})
file(COPY ${R_PKG_IN_DIR}/NAMESPACE DESTINATION ${R_PKG_OUT_DIR})
file(COPY ${R_PKG_IN_DIR}/src/qualpalr.cpp DESTINATION ${R_PKG_OUT_DIR}/src)

# Copy files to build directory
add_custom_target(r_copy ALL COMMENT "Copy source files to build directory")

# Copy R files
file(
  GLOB_RECURSE R_FILES
  RELATIVE ${R_PKG_IN_DIR}
  ${R_PKG_IN_DIR}/*.R ${R_PKG_IN_DIR}/*.RData)

file(
  GLOB_RECURSE CPP_SRC_HEADERS
  RELATIVE ${CPP_SRC_DIR}
  CONFIGURE_DEPENDS ${CPP_SRC_DIR}/*.h)

file(
  GLOB_RECURSE CPP_SRC_SOURCES
  RELATIVE ${CPP_SRC_DIR}
  CONFIGURE_DEPENDS ${CPP_SRC_DIR}/*.cpp)

file(
  GLOB_RECURSE RCPP_SOURCES
  RELATIVE ${R_PKG_IN_DIR}/src
  CONFIGURE_DEPENDS ${R_PKG_IN_DIR}/*.cpp)

set(CPP_SRC_FILES ${CPP_SRC_HEADERS} ${CPP_SRC_SOURCES})

# Configure DESCRIPTION and LICENSE
configure_file(${R_PKG_IN_DIR}/DESCRIPTION.in ${R_PKG_OUT_DIR}/DESCRIPTION
               @ONLY)
configure_file(${R_PKG_IN_DIR}/LICENSE.in ${R_PKG_OUT_DIR}/LICENSE @ONLY)

# Write the Makevars file
set(MAKEVARS_FILE "${R_PKG_OUT_DIR}/src/Makevars")

file(WRITE ${MAKEVARS_FILE} "SOURCES = \\\n\tRcppExports.cpp")

foreach(file ${RCPP_SOURCES})
  file(APPEND ${MAKEVARS_FILE} " \\\n\t${file}")
endforeach()

foreach(file ${CPP_SRC_SOURCES})
  file(APPEND ${MAKEVARS_FILE} " \\\n\t${CMAKE_PROJECT_NAME}/${file}")
endforeach()

file(
  APPEND ${MAKEVARS_FILE}
  "\n\n"
  "OBJECTS = $(SOURCES:.cpp=.o)\n"
  "\n"
  "all: $(SHLIB)\n"
  "\n"
  "clean:\n"
  "\t@rm -f $(OBJECTS)")

# Copy all files over to the build directory
foreach(file ${R_FILES})
  get_filename_component(PARENT_DIR "${file}" DIRECTORY)
  file(MAKE_DIRECTORY "${R_PKG_OUT_DIR}/${PARENT_DIR}")
  add_custom_command(
    TARGET r_copy
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${R_PKG_IN_DIR}/${file}
            ${R_PKG_OUT_DIR}/${file})
endforeach()

foreach(file ${CPP_SRC_FILES})
  add_custom_command(
    TARGET r_copy
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CPP_SRC_DIR}/${file}
            ${R_PKG_OUT_DIR}/src/${CMAKE_PROJECT_NAME})
endforeach()

add_custom_target(r_build ALL COMMENT "Add target for R package build")

add_custom_command(
  TARGET r_build
  POST_BUILD
  COMMAND ${RSCRIPT_EXECUTABLE} ARGS "-e" "'Rcpp::compileAttributes()'"
  WORKING_DIRECTORY "${R_PKG_OUT_DIR}"
  COMMENT "Call Rcpp::compileAttributes() to generate bindings to R.")

add_custom_command(
  TARGET r_build
  POST_BUILD
  COMMAND ${RSCRIPT_EXECUTABLE} ARGS "-e" "'roxygen2::roxygenize(\".\")'"
  WORKING_DIRECTORY "${R_PKG_OUT_DIR}"
  COMMENT "Document the R package code using roxygen2.")

add_custom_command(
  TARGET r_build
  POST_BUILD
  COMMAND ${R_EXECUTABLE} CMD build ${R_PKG_NAME}
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  COMMENT "Build the R package.")

install(
  CODE "execute_process(COMMAND Rscript -e \"install.packages('${R_PKG_NAME}_${R_PKG_VERSION}.tar.gz', repos=NULL)\"
                        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})"
  COMPONENT R_BINDINGS
  EXCLUDE_FROM_ALL)
