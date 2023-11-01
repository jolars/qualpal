#pragma once

#include <string>

namespace qualpal {

// forward declarations
class HSL;
class XYZ;

class RGB
{
private:
  double r;
  double g;
  double b;

public:
  RGB(const double r, const double g, const double b);
  RGB(const std::string& hex);
  RGB(const HSL& hsl);
  RGB(const XYZ& xyz);
  std::string hex() const;
  double red() const { return r; }
  double green() const { return g; }
  double blue() const { return b; }
};

} // namespace qualpal
