#include "rgb.h"
#include "hsl.h"
#include "matrix.h"
#include "xyz.h"
#include <cmath>
#include <iomanip>
#include <sstream>

namespace qualpal {
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

} // namespace qualpal
