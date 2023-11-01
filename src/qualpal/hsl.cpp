#include "hsl.h"
#include <algorithm>
#include <cmath>

namespace qualpal {

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

} // namespace qualpal
