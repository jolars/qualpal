#include "color_grid.h"
#include <array>
#include <cmath>

namespace qualpal {
std::vector<HSL>
colorGrid(int n,
          const std::array<double, 2>& h_lim,
          const std::array<double, 2>& s_lim,
          const std::array<double, 2>& l_lim)
{
  std::vector<HSL> colors;
  colors.reserve(n);

  double h_range = h_lim[1] - h_lim[0];
  double s_range = s_lim[1] - s_lim[0];
  double l_range = l_lim[1] - l_lim[0];
  double total_range = h_range + s_range + l_range;

  int n_h = std::round(n * h_range / total_range);
  int n_s = std::round(n * s_range / total_range);
  int n_l = n - n_h - n_s;

  double h_step = h_range / static_cast<double>(n_h - 1);
  double s_step = s_range / static_cast<double>(n_s - 1);
  double l_step = l_range / static_cast<double>(n_l - 1);

  for (int i = 0; i < n_h; i++) {
    for (int j = 0; j < n_s; j++) {
      for (int k = 0; k < n_l; k++) {
        double h = h_lim[0] + h_step * i;
        double s = s_lim[0] + s_step * j;
        double l = l_lim[0] + l_step * k;
        colors.emplace_back(h, s, l);
      }
    }
  }

  return colors;
}

} // namespace qualpal
