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
  : r_value(r)
  , g_value(g)
  , b_value(b)
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

  r_value = red / 255.0;
  g_value = green / 255.0;
  b_value = blue / 255.0;
}

RGB::RGB(const HSL& hsl)
{
  double c = (1 - std::abs(2 * hsl.l() - 1)) * hsl.s();
  double h_prime = hsl.h() >= 0 ? hsl.h() / 60.0 : (hsl.h() - 360) / 60.0;
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

  double m = hsl.l() - c / 2.0;

  r_value = rgb_prime[0] + m;
  g_value = rgb_prime[1] + m;
  b_value = rgb_prime[2] + m;
}

XYZ::XYZ(const double x, const double y, const double z)
  : x_value(x)
  , y_value(y)
  , z_value(z)
{
}

XYZ::XYZ(const RGB& rgb)
{
  FixedMatrix<double, 3, 3> m = { { 0.4124564, 0.3575761, 0.1804375 },
                                  { 0.2126729, 0.7151522, 0.0721750 },
                                  { 0.0193339, 0.1191920, 0.9503041 } };

  double r = inverseCompanding(rgb.r());
  double g = inverseCompanding(rgb.g());
  double b = inverseCompanding(rgb.b());

  std::array<double, 3> rgb_vec = { r, g, b };
  auto xyz = m * rgb_vec;

  x_value = xyz[0];
  y_value = xyz[1];
  z_value = xyz[2];
}

XYZ::XYZ(const Lab& lab, const std::array<double, 3>& white_point)
{
  const double l = lab.l();
  const double a = lab.a();
  const double b = lab.b();

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

  x_value = xr * white_point[0];
  y_value = yr * white_point[1];
  z_value = zr * white_point[2];
}

RGB::RGB(const XYZ& xyz)
{
  FixedMatrix<double, 3, 3> m = { { 3.2404542, -1.5371385, -0.4985314 },
                                  { -0.9692660, 1.8760108, 0.0415560 },
                                  { 0.0556434, -0.2040259, 1.0572252 } };
  std::array<double, 3> xyz_vec = { xyz.x(), xyz.y(), xyz.z() };

  auto rgb = m * xyz_vec;

  for (auto& val : rgb) {
    if (val > 0.0031308) {
      val = 1.055 * std::pow(val, 1 / 2.4) - 0.055;
    } else {
      val = 12.92 * val;
    }
  }

  r_value = rgb[0];
  g_value = rgb[1];
  b_value = rgb[2];
}

std::string
RGB::hex() const
{
  std::stringstream ss;

  ss << "#" << std::hex << std::setfill('0');
  ss << std::setw(2) << static_cast<int>(this->r() * 255);
  ss << std::setw(2) << static_cast<int>(this->g() * 255);
  ss << std::setw(2) << static_cast<int>(this->b() * 255);

  return ss.str();
}

DIN99d::DIN99d(const double l, const double a, const double b)
  : l_value(l)
  , a_value(a)
  , b_value(b)
{
}

DIN99d::DIN99d(const XYZ& xyz)
{
  double new_x = 1.12 * xyz.x() - 0.12 * xyz.z();

  XYZ xyz_prime(new_x, xyz.y(), xyz.z());

  Lab lab(xyz_prime, { 0.95047, 1.0, 1.08883 });

  double l = lab.l();
  double a = lab.a();
  double b = lab.b();

  double u = 50 * M_PI / 180.0;
  double e = a * std::cos(u) + b * std::sin(u);
  double f = 1.14 * (b * std::cos(u) - a * std::sin(u));
  double g = std::sqrt(e * e + f * f);

  double c99d = 22.5 * std::log(1.0 + 0.06 * g);
  double h99d = std::atan2(f, e) + 50 * M_PI / 180.0;

  l_value = 325.22 * std::log(1.0 + 0.0036 * l);
  a_value = c99d * std::cos(h99d);
  b_value = c99d * std::sin(h99d);
}

DIN99d::DIN99d(const RGB& rgb)
  : DIN99d(XYZ(rgb))
{
}

HSL::HSL(const double h, const double s, const double l)
  : h_value(h)
  , s_value(s)
  , l_value(l)
{
}

HSL::HSL(const RGB& rgb)
{
  const double r = rgb.r();
  const double g = rgb.g();
  const double b = rgb.b();

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

  h_value = h_prime * 60;
  l_value = 0.5 * (M + m);
  s_value =
    (l_value == 1 || l_value == 0) ? 0 : C / (1.0 - std::abs(2 * l_value - 1));
}

Lab::Lab(const double l, const double a, const double b)
  : l_value(l)
  , a_value(a)
  , b_value(b)
{
}

Lab::Lab(const XYZ& xyz, const std::array<double, 3>& white_point)
{
  double x = xyz.x();
  double y = xyz.y();
  double z = xyz.z();

  double epsilon = 216.0 / 24389.0;
  double kappa = 24389.0 / 27.0;

  double xr = x / white_point[0];
  double yr = y / white_point[1];
  double zr = z / white_point[2];

  double fx = xr > epsilon ? std::cbrt(xr) : (kappa * xr + 16.0) / 116.0;
  double fy = yr > epsilon ? std::cbrt(yr) : (kappa * yr + 16.0) / 116.0;
  double fz = zr > epsilon ? std::cbrt(zr) : (kappa * zr + 16.0) / 116.0;

  l_value = 116.0 * fy - 16.0;
  a_value = 500.0 * (fx - fy);
  b_value = 200.0 * (fy - fz);
};

} // namespace qualpal
