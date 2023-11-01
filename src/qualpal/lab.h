#pragma once

#include "xyz.h"
#include <array>

namespace qualpal {
class Lab
{
private:
  double l;
  double a;
  double b;

public:
  Lab(const double l, const double a, const double b);
  Lab(const XYZ& xyz,
      const std::array<double, 3>& white_point = { 0.95047, 1, 1.08883 });
  double lightness() const { return l; }
  double a_value() const { return a; }
  double b_value() const { return b; }
};

} // namespace qualpal
