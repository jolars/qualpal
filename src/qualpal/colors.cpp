#include "colors.h"
#include "matrix.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace qualpal {

inline double
inverseCompanding(const double v)
{
  return v <= 0.04045 ? v / 12.92 : std::pow((v + 0.055) / 1.055, 2.4);
}

RGB::RGB(const double r, const double g, const double b)
  : r(r)
  , g(g)
  , b(b)
{
}

RGB::RGB(const std::string& hex)
{
  std::stringstream ss;

  double red = 0;
  double green = 0;
  double blue = 0;

  if (hex.length() == 4) {
    ss << std::hex << hex.substr(1, 1) << hex.substr(1, 1);
    ss >> red;

    ss.clear();

    ss << std::hex << hex.substr(2, 1) << hex.substr(2, 1);
    ss >> green;

    ss.clear();

    ss << std::hex << hex.substr(3, 1) << hex.substr(3, 1);
    ss >> blue;
  } else if (hex.length() == 7) {
    ss << std::hex << hex.substr(1, 2);
    ss >> red;

    ss.clear();

    ss << std::hex << hex.substr(3, 2);
    ss >> green;

    ss.clear();

    ss << std::hex << hex.substr(5, 2);
    ss >> blue;
  }

  r = red / 255.0;
  g = green / 255.0;
  b = blue / 255.0;
}

RGB::RGB(const HSL& hsl)
{
  double c = (1 - std::abs(2 * hsl.lightness() - 1)) * hsl.saturation();
  double h_prime = hsl.hue() >= 0 ? hsl.hue() / 60.0 : (hsl.hue() - 360) / 60.0;
  double x = c * (1 - std::abs(std::fmod(h_prime, 2) - 1));

  double rgb_prime[3] = { 0, 0, 0 };

  if (h_prime >= 0 && h_prime < 1) {
    rgb_prime[0] = c;
    rgb_prime[1] = x;
  } else if (h_prime >= 1 && h_prime < 2) {
    rgb_prime[0] = x;
    rgb_prime[1] = c;
  } else if (h_prime >= 2 && h_prime < 3) {
    rgb_prime[1] = c;
    rgb_prime[2] = x;
  } else if (h_prime >= 3 && h_prime < 4) {
    rgb_prime[1] = x;
    rgb_prime[2] = c;
  } else if (h_prime >= 4 && h_prime < 5) {
    rgb_prime[0] = x;
    rgb_prime[2] = c;
  } else if (h_prime >= 5 && h_prime < 6) {
    rgb_prime[0] = c;
    rgb_prime[2] = x;
  }

  double m = hsl.lightness() - c / 2.0;

  r = rgb_prime[0] + m;
  g = rgb_prime[1] + m;
  b = rgb_prime[2] + m;
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

RGB::RGB(const XYZ& xyz)
{
  FixedMatrix<double, 3, 3> m = { { 3.2404542, -1.5371385, -0.4985314 },
                                  { -0.9692660, 1.8760108, 0.0415560 },
                                  { 0.0556434, -0.2040259, 1.0572252 } };
  std::array<double, 3> xyz_vec = { xyz.x_value(),
                                    xyz.y_value(),
                                    xyz.z_value() };

  auto rgb = m * xyz_vec;

  for (auto& val : rgb) {
    if (val > 0.0031308) {
      val = 1.055 * std::pow(val, 1 / 2.4) - 0.055;
    } else {
      val = 12.92 * val;
    }
  }

  r = rgb[0];
  g = rgb[1];
  b = rgb[2];
}

std::string
RGB::hex() const
{
  std::stringstream ss;

  ss << "#" << std::hex << std::setfill('0');
  ss << std::setw(2) << static_cast<int>(r * 255);
  ss << std::setw(2) << static_cast<int>(g * 255);
  ss << std::setw(2) << static_cast<int>(b * 255);

  return ss.str();
}

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

HSL::HSL(const double h, const double s, const double l)
  : h(h)
  , s(s)
  , l(l)
{
}

HSL::HSL(const RGB& rgb)
{
  const double r = rgb.red();
  const double g = rgb.green();
  const double b = rgb.blue();

  const double M = std::max({ r, g, b });
  const double m = std::min({ r, g, b });
  const double C = M - m;

  double h_prime = 0;

  if (C == 0) {
    h_prime = std::nan("");
  } else if (M == r) {
    h_prime = std::fmod((g - b) / C, 6.0);
  } else if (M == g) {
    h_prime = (b - r) / C + 2;
  } else if (M == b) {
    h_prime = (r - g) / C + 4;
  }

  h = h_prime * 60;
  l = 0.5 * (M + m);
  s = (l == 1 || l == 0) ? 0 : C / (1.0 - std::abs(2 * l - 1));
}

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
