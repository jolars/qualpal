#pragma once

#include <array>
#include <string>

namespace qualpal {

// forward declarations
class HSL;
class XYZ;
class Lab;

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
