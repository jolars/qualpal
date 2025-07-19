#include <array>
#include <qualpal/colors.h>
#include <vector>

namespace qualpal {

template<int Dims>
class Halton
{
public:
  Halton()
  {
    static_assert(Dims <= 25, "Dimensions of more than 25 are not supported");
  }

  std::array<double, Dims> next()
  {
    std::array<double, Dims> result;

    for (int i = 0; i < Dims; i++) {
      result[i] = halton(counter, primes[i]);
    }

    ++counter;

    return result;
  }

private:
  double halton(const int index, const int base)
  {
    double f = 1.0;
    double result = 0;

    int i = index;

    while (i > 0) {
      f = f / base;
      result = result + f * (i % base);
      i = i / base;
    }

    return result;
  }

  unsigned int counter = 0;
  const std::array<int, 25> primes = { 2,  3,  5,  7,  11, 13, 17, 19, 23,
                                       29, 31, 37, 41, 43, 47, 53, 59, 61,
                                       67, 71, 73, 79, 83, 89, 97 };
};

// Scale x from [0, 1] to [min, max]
inline double
scaleToInterval(const double x, const double min, const double max)
{
  return (max - min) * (x - 1) + max;
}

template<typename ColorType>
std::vector<ColorType>
colorGrid(const std::array<double, 2>& angle_lim,
          const std::array<double, 2>& radius_lim,
          const std::array<double, 2>& height_lim,
          int n_points)
{
  std::vector<ColorType> colors;
  colors.reserve(n_points);
  Halton<3> halton;

  for (int i = 0; i < n_points; ++i) {
    auto vals = halton.next();
    double phi = scaleToInterval(vals[0], angle_lim[0], angle_lim[1]);
    double r = scaleToInterval(vals[1], radius_lim[0], radius_lim[1]);
    double h = scaleToInterval(vals[2], height_lim[0], height_lim[1]);
    colors.emplace_back(phi < 0 ? phi + 360 : phi, r, h);
  }

  return colors;
}

} // namespace qualpal
