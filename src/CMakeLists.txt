add_library(qualpal qualpal/qualpal.cpp qualpal/colors.cpp
                    qualpal/distance_matrix.cpp qualpal/farthest_points.cpp)

target_include_directories(qualpal INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
