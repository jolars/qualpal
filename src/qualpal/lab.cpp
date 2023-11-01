#include "lab.h"
#include <cmath>

namespace qualpal {

Lab::Lab(const double l, const double a, const double b)
  : l(l)
  , a(a)
  , b(b)
{
}

Lab::Lab(const XYZ& xyz, const std::array<double, 3>& white_point)
{
  double x = xyz.x_value();
  double y = xyz.y_value();
  double z = xyz.z_value();

  double epsilon = 216.0 / 24389.0;
  double kappa = 24389.0 / 27.0;

  double xr = x / white_point[0];
  double yr = y / white_point[1];
  double zr = z / white_point[2];

  double fx = xr > epsilon ? std::cbrt(xr) : (kappa * xr + 16.0) / 116.0;
  double fy = yr > epsilon ? std::cbrt(yr) : (kappa * yr + 16.0) / 116.0;
  double fz = zr > epsilon ? std::cbrt(zr) : (kappa * zr + 16.0) / 116.0;

  l = 116.0 * fy - 16.0;
  a = 500.0 * (fx - fy);
  b = 200.0 * (fy - fz);
};

} // namespace qualpal
