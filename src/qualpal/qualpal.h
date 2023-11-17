#include "colors.h"
#include <vector>

namespace qualpal {

std::vector<RGB>
qualpal(const int n, const std::vector<RGB>& rgb_colors);

std::vector<RGB>
qualpal(const int n, const std::vector<std::string>& hex_colors);

} // namespace qualpal
