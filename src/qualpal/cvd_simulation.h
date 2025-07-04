#include <cmath>
#include <qualpal_bits/colors.h>
#include <string>
#include <vector>

namespace qualpal {

std::vector<RGB>
simulate_cvd(const std::vector<RGB>& rgb,
             const std::string cvd_type,
             double cvd_severity);

}
