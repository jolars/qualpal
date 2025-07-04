add_library(
    qualpal
    qualpal/qualpal.cpp
    qualpal/colors.cpp
    qualpal/distance_matrix.cpp
    qualpal/farthest_points.cpp
    qualpal/color_grid.cpp
    qualpal/cvd_simulation.cpp
    qualpal/validation.cpp
    qualpal/palettes.cpp
)

target_include_directories(qualpal INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS qualpal DESTINATION lib)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qualpal/
    DESTINATION include/qualpal
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)
