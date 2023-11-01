#include "din99d.h"
#include "lab.h"
#include "xyz.h"
#include <cmath>

namespace qualpal {

DIN99d::DIN99d(const double l99, const double a99, const double b99)
  : l99(l99)
  , a99(a99)
  , b99(b99)
{
}

DIN99d::DIN99d(const XYZ& xyz)
{
  double new_x = 1.12 * xyz.x_value() - 0.12 * xyz.z_value();

  XYZ xyz_prime(new_x, xyz.y_value(), xyz.z_value());

  Lab lab(xyz_prime, { 0.95047, 1.0, 1.08883 });

  double l = lab.lightness();
  double a = lab.a_value();
  double b = lab.b_value();

  double u = 50 * M_PI / 180.0;
  double e = a * std::cos(u) + b * std::sin(u);
  double f = 1.14 * (b * std::cos(u) - a * std::sin(u));
  double g = std::sqrt(e * e + f * f);

  double c99d = 22.5 * std::log(1.0 + 0.06 * g);
  double h99d = std::atan2(f, e) + 50 * M_PI / 180.0;

  l99 = 325.22 * std::log(1.0 + 0.0036 * l);
  a99 = c99d * std::cos(h99d);
  b99 = c99d * std::sin(h99d);
}

DIN99d::DIN99d(const RGB& rgb)
  : DIN99d(XYZ(rgb))
{
}

} // namespace qualpal
