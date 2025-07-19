#include "../src/qualpal/color_grid.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal/color_difference.h>
#include <qualpal/metrics.h>

TEST_CASE("Color difference matrices", "[colordiff]")
{
  using namespace qualpal;
  using namespace qualpal::colors;
  using namespace Catch::Matchers;

  std::array<double, 2> h_lim = { 50.0, 360.0 };
  std::array<double, 2> s_lim = { 0.2, 0.8 };
  std::array<double, 2> l_lim = { 0.3, 0.7 };

  int n_points = 20;

  auto colors = colorGrid<HSL>(h_lim, s_lim, l_lim, n_points);

  REQUIRE_NOTHROW(
    qualpal::colorDifferenceMatrix(colors, qualpal::metrics::DIN99d{}));
  REQUIRE_NOTHROW(
    qualpal::colorDifferenceMatrix(colors, qualpal::metrics::CIEDE2000{}));
  REQUIRE_NOTHROW(
    qualpal::colorDifferenceMatrix(colors, qualpal::metrics::CIE76{}));
}
