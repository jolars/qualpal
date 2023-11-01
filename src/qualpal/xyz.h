#pragma once

#include "rgb.h"
#include <array>

namespace qualpal {

class Lab;

class XYZ
{
private:
  double x;
  double y;
  double z;

public:
  XYZ(const double x, const double y, const double z);
  XYZ(const RGB& rgb);
  XYZ(const Lab& lab,
      const std::array<double, 3>& white_point = { 0.95047, 1, 1.08883 });
  double x_value() const { return x; }
  double y_value() const { return y; }
  double z_value() const { return z; }
};

} // namespace qualpal
