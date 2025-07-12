#include <cmath>
#include <qualpal/colors.h>
#include <string>
#include <vector>

namespace qualpal {

std::vector<colors::RGB>
simulateCvd(const std::vector<colors::RGB>& rgb,
            const std::string cvd_type,
            double cvd_severity);

}
