add_library(qualpal_lib qualpal/qualpal.cpp qualpal/colors.cpp)

target_include_directories(qualpal_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
