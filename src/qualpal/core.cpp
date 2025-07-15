#include "color_grid.h"
#include "cvd.h"
#include "farthest_points.h"
#include "palettes.h"
#include "validation.h"
#include <cassert>
#include <map>
#include <qualpal/core.h>
#include <stdexcept>
#include <vector>

namespace qualpal {

std::vector<colors::RGB>
qualpal(const int n,
        const std::vector<colors::RGB>& rgb_colors,
        const std::map<std::string, double>& cvd,
        const std::optional<colors::RGB>& bg,
        const metrics::MetricType& metric,
        const size_t max_memory)
{
  int n_colors = rgb_colors.size();

  std::vector<colors::RGB> rgb_colors_mod = rgb_colors;
  std::optional<colors::RGB> bg_mod = bg;

  for (const auto& [cvd_type, cvd_severity] : cvd) {
    if (cvd_severity > 1.0 || cvd_severity < 0.0) {
      throw std::invalid_argument("cvd_severity must be between 0 and 1");
    }

    if (cvd_severity > 0) {
      for (auto& rgb : rgb_colors_mod) {
        rgb = simulateCvd(rgb, cvd_type, cvd_severity);
      }

      if (bg_mod.has_value()) {
        bg_mod = simulateCvd(*bg_mod, cvd_type, cvd_severity);
      }
    }
  }

  std::vector<colors::Lab> lab_colors;
  lab_colors.reserve(n_colors);

  for (const auto& rgb : rgb_colors_mod) {
    lab_colors.emplace_back(rgb);
  }

  auto ind = farthestPoints(n, lab_colors, metric, bg_mod, max_memory);

  std::vector<colors::RGB> rgb_out;
  rgb_out.reserve(n);

  for (const auto& i : ind) {
    rgb_out.emplace_back(rgb_colors[i]);
  }

  return rgb_out;
}

std::vector<colors::RGB>
qualpal(const int n,
        const std::vector<std::string>& hex_colors,
        const std::map<std::string, double>& cvd,
        const std::optional<colors::RGB>& bg,
        const metrics::MetricType& metric,
        const size_t max_memory)
{
  std::vector<colors::RGB> rgb_colors;

  for (auto color : hex_colors) {
    assert(isValidHexColor(color) && "Values are hex colors");
    rgb_colors.emplace_back(color);
  }

  return qualpal(n, rgb_colors, cvd, bg, metric, max_memory);
}

std::vector<colors::RGB>
qualpal(const int n,
        const std::string& palette,
        const std::map<std::string, double>& cvd,
        const std::optional<colors::RGB>& bg,
        const metrics::MetricType& metric,
        const size_t max_memory)
{
  std::vector<colors::RGB> rgb_colors;

  std::vector<std::string> hex_colors = getPalette(palette);

  if (n > static_cast<int>(hex_colors.size())) {
    throw std::invalid_argument("Requested number of colors (" +
                                std::to_string(n) + ") exceeds palette size (" +
                                std::to_string(hex_colors.size()) + ")");
  }

  for (auto color : hex_colors) {
    rgb_colors.emplace_back(color);
  }

  return qualpal(n, rgb_colors, cvd, bg, metric, max_memory);
}

std::vector<colors::RGB>
qualpal(const int n,
        const std::array<double, 2>& h_lim,
        const std::array<double, 2>& s_lim,
        const std::array<double, 2>& l_lim,
        const int n_points,
        const std::map<std::string, double>& cvd,
        const std::optional<colors::RGB>& bg,
        const metrics::MetricType& metric,
        const size_t max_memory)
{
  if (n > n_points) {
    throw std::invalid_argument("Number of colors to generate must be lower "
                                "than the number of points in the color grid");
  }

  if (h_lim[0] < -360 || h_lim[1] > 360) {
    throw std::invalid_argument("Hue must be between -360 and 360");
  }

  if (h_lim[1] - h_lim[0] > 360) {
    throw std::invalid_argument("Hue range must be less than 360");
  }

  if (s_lim[0] < 0 || s_lim[1] > 1) {
    throw std::invalid_argument("Saturation must be between 0 and 1");
  }

  if (l_lim[0] < 0 || l_lim[1] > 1) {
    throw std::invalid_argument("Lightness must be between 0 and 1");
  }

  auto hsl_colors = colorGrid(h_lim, s_lim, l_lim, n_points);

  std::vector<qualpal::colors::RGB> rgb_colors;

  for (auto color : hsl_colors) {
    rgb_colors.emplace_back(color);
  }

  return qualpal(n, rgb_colors, cvd, bg, metric, max_memory);
}

} // namespace qualpal
