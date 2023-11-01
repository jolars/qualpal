#include "colors.h"
#include <vector>

namespace qualpal {
std::vector<HSL>
hslGrid(int n,
        double h_start = 0,
        double h_end = 360,
        double s_start = 0,
        double s_end = 1,
        double l_start = 0,
        double l_end = 1);

} // namespace qualpal
