#include "xyz.h"
#include "lab.h"
#include "matrix.h"
#include <cmath>

namespace qualpal {

inline double
inverseCompanding(const double v)
{
  return v <= 0.04045 ? v / 12.92 : std::pow((v + 0.055) / 1.055, 2.4);
}

XYZ::XYZ(const double x, const double y, const double z)
  : x(x)
  , y(y)
  , z(z)
{
}

XYZ::XYZ(const RGB& rgb)
{
  FixedMatrix<double, 3, 3> m = { { 0.4124564, 0.3575761, 0.1804375 },
                                  { 0.2126729, 0.7151522, 0.0721750 },
                                  { 0.0193339, 0.1191920, 0.9503041 } };

  double r = inverseCompanding(rgb.red());
  double g = inverseCompanding(rgb.green());
  double b = inverseCompanding(rgb.blue());

  std::array<double, 3> rgb_vec = { r, g, b };
  auto xyz = m * rgb_vec;

  x = xyz[0];
  y = xyz[1];
  z = xyz[2];
}

XYZ::XYZ(const Lab& lab, const std::array<double, 3>& white_point)
{
  const double l = lab.lightness();
  const double a = lab.a_value();
  const double b = lab.b_value();

  const double epsilon = 216.0 / 24389.0;
  const double kappa = 24389.0 / 27.0;

  double fy = (l + 16.0) / 116.0;
  double fx = a / 500.0 + fy;
  double fz = fy - b / 200.0;

  double xr =
    std::pow(fx, 3) > epsilon ? std::pow(fx, 3) : (116.0 * fx - 16.0) / kappa;
  double yr = l > kappa * epsilon ? std::pow((l + 16.0) / 116.0, 3) : l / kappa;
  double zr =
    std::pow(fz, 3) > epsilon ? std::pow(fz, 3) : (116.0 * fz - 16.0) / kappa;

  x = xr * white_point[0];
  y = yr * white_point[1];
  z = zr * white_point[2];
}

} // namespace qualpal
