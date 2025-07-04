/**
 * @file
 * @brief Main qualpal API for generating qualitative color palettes
 *
 * This file contains the core functions for generating maximally distinct
 * color palettes using perceptual color difference in DIN99d color space.
 * Supports multiple input formats and color vision deficiency simulation.
 */

#include "colors.h"
#include "distance_matrix.h"
#include <map>
#include <vector>

/**
 * @namespace qualpal
 * @brief Qualitative color palette generation library
 *
 * The qualpal namespace contains all functionality for generating
 * perceptually distinct color palettes. The main entry points are
 * the overloaded qualpal() functions that accept different input formats.
 *
 * @see qualpal() for the main palette generation functions
 */
namespace qualpal {

/**
 * @brief Generate qualitative color palette from existing colors
 *
 * Selects n maximally distinct colors from the input set using
 * perceptual color difference in DIN99d color space.
 *
 * @param n Number of colors to select
 * @param rgb_colors Input RGB colors to choose from
 * @param cvd Color vision deficiency simulation parameters
 *            Map of {"protanomaly"|"deuteranomaly"|"tritanomaly" -> severity
 * [0,1]}
 * @return Vector of n selected RGB colors
 *
 * @throws std::invalid_argument if cvd severity not in [0,1]
 *
 * @code{.cpp}
 * std::vector<RGB> colors = {RGB(1,0,0), RGB(0,1,0), RGB(0,0,1)};
 * auto palette = qualpal(2, colors);
 * @endcode
 */
std::vector<RGB>
qualpal(const int n,
        std::vector<RGB> rgb_colors,
        const std::map<std::string, double>& cvd = {});

/**
 * @brief Generate qualitative color palette from hex color strings
 *
 * Convenience overload that accepts hex color strings and converts
 * them to RGB internally before palette generation.
 *
 * @param n Number of colors to select
 * @param hex_colors Vector of hex color strings (e.g., "#ff0000")
 * @param cvd Color vision deficiency simulation parameters
 * @return Vector of n selected RGB colors
 *
 * @throws std::invalid_argument if hex colors are invalid or cvd severity not
 * in [0,1]
 *
 * @code{.cpp}
 * std::vector<std::string> colors = {"#ff0000", "#00ff00", "#0000ff"};
 * auto palette = qualpal(2, colors);
 * @endcode
 */
std::vector<RGB>
qualpal(const int n,
        const std::vector<std::string>& hex_colors,
        const std::map<std::string, double>& cvd = {});

/**
 * @brief Generate qualitative color palette from named palette
 *
 * Selects colors from a built-in color palette by name.
 * Available palettes include standard sets like "viridis", "rainbow", etc.
 *
 * @param n Number of colors to select
 * @param palette Name of the built-in palette to use, of the form
 *   "Package:Palette"
 * @param cvd Color vision deficiency simulation parameters
 * @return Vector of n selected RGB colors
 *
 * @throws std::invalid_argument if palette name unknown, n exceeds palette
 * size, or cvd severity not in [0,1]
 *
 * @code{.cpp}
 * auto palette = qualpal(5, "ColorBrewer:Set1");
 * @endcode
 */
std::vector<RGB>
qualpal(const int n,
        const std::string& palette,
        const std::map<std::string, double>& cvd = {});

/**
 * @brief Generate palette from HSL color space sampling
 *
 * Creates a grid of colors in HSL space within the specified ranges
 * and selects n maximally distinct colors.
 *
 * @param n Number of colors to generate
 * @param h_lim Hue range in degrees [-360, 360]
 * @param s_lim Saturation range [0, 1]
 * @param l_lim Lightness range [0, 1]
 * @param n_points Number of grid points to sample (default 100)
 * @param cvd Color vision deficiency parameters
 * @return Vector of n generated RGB colors
 *
 * @throws std::invalid_argument for invalid ranges or n > n_points
 */
std::vector<RGB>
qualpal(const int n,
        const std::array<double, 2>& h_lim = { 0, 360 },
        const std::array<double, 2>& s_lim = { 0, 1 },
        const std::array<double, 2>& l_lim = { 0, 1 },
        const int n_points = 100,
        const std::map<std::string, double>& cvd = {});

} // namespace qualpal
