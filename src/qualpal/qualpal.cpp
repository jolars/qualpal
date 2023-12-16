#include "qualpal.h"
#include "color_grid.h"
#include "farthest_points.h"
#include <cassert>
#include <regex>
#include <vector>

namespace qualpal {

// Check if the input is a valid HTML hex color representation
bool
isValidHexColor(const std::string& color)
{
  std::regex color_regex("^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$");
  return std::regex_match(color, color_regex);
}

std::vector<RGB>
qualpal(const int n, const std::vector<RGB>& rgb_colors)
{
  int N = rgb_colors.size();

  std::vector<DIN99d> din99d_colors;
  din99d_colors.reserve(N);

  for (const auto& rgb : rgb_colors) {
    din99d_colors.emplace_back(rgb);
  }

  auto ind = farthestPoints(n, din99d_colors);

  std::vector<RGB> rgb_out;
  rgb_out.reserve(ind.size());

  for (const auto& i : ind) {
    rgb_out.emplace_back(rgb_colors[i]);
  }

  return rgb_out;
}

std::vector<RGB>
qualpal(const int n, const std::vector<std::string>& hex_colors)
{
  std::vector<qualpal::RGB> rgb_colors;

  for (auto color : hex_colors) {
    assert(isValidHexColor(color) && "Values are hex colors");
    rgb_colors.emplace_back(color);
  }

  return qualpal(n, rgb_colors);
}

// std::vector<RGB>
// qualpal(const int n,
//         const std::array<double, 2>& h_lim,
//         const std::array<double, 2>& s_lim,
//         const std::array<double, 2>& l_lim)
// {
//   auto hsl_colors = colorGrid(n, h_lim, s_lim, l_lim);
//
//   std::vector<qualpal::RGB> rgb_colors;
//
//   for (auto color : hsl_colors) {
//     rgb_colors.emplace_back(color);
//   }
//
//   return qualpal(n, rgb_colors);
// }

} // namespace qualpal
