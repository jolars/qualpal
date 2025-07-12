#include <cmath>
#include <qualpal/colors.h>
#include <string>

namespace qualpal {

colors::RGB
simulateCvd(const colors::RGB& rgb,
            const std::string cvd_type,
            double cvd_severity);

}
