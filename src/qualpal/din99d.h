#pragma once

#include "rgb.h"
#include "xyz.h"

namespace qualpal {

class DIN99d
{
private:
  double l99;
  double a99;
  double b99;

public:
  DIN99d(const double l99, const double a99, const double b99);
  DIN99d(const XYZ& xyz);
  DIN99d(const RGB& rgb);
  double lightness() const { return l99; }
  double a() const { return a99; }
  double b() const { return b99; }
};

} // namespace qualpal
