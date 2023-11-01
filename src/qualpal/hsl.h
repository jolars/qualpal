#pragma once

#include "rgb.h"

namespace qualpal {

class HSL
{
private:
  double h;
  double s;
  double l;

public:
  HSL(const double h, const double s, const double l);
  HSL(const RGB& rgb);
  double hue() const { return h; }
  double saturation() const { return s; }
  double lightness() const { return l; }
};

} // namespace qualpal
