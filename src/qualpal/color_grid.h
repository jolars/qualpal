#include "colors.h"
#include <array>
#include <vector>

namespace qualpal {
std::vector<HSL>
colorGrid(int n,
          const std::array<double, 2>& h_lim,
          const std::array<double, 2>& s_lim,
          const std::array<double, 2>& l_lim);

} // namespace qualpal
