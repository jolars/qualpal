#include "color_grid.h"
#include <array>
#include <cmath>

namespace qualpal {
std::vector<HSL>
colorGrid(int n,
          double h_start,
          double h_end,
          double s_start,
          double s_end,
          double l_start,
          double l_end)
{
  std::vector<HSL> colors;
  colors.reserve(n);

  double h_range = h_end - h_start;
  double s_range = s_end - s_start;
  double l_range = l_end - l_start;
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
        double h = h_start + h_step * i;
        double s = s_start + s_step * j;
        double l = l_start + l_step * k;
        colors.emplace_back(h, s, l);
      }
    }
  }

  return colors;
}

} // namespace qualpal
