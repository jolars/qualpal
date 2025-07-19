#include "../src/qualpal/color_grid.h"
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal/colors.h>

using namespace qualpal;

TEST_CASE("colorGrid generates correct number of HSL colors", "[colorGrid]")
{
  using namespace qualpal;

  std::array<double, 2> hue_lim{ 0, 360 };
  std::array<double, 2> sat_lim{ 0, 1 };
  std::array<double, 2> light_lim{ 0, 1 };
  int n_points = 50;

  auto colors = colorGrid<colors::HSL>(hue_lim, sat_lim, light_lim, n_points);

  REQUIRE(colors.size() == n_points);
}

TEST_CASE("colorGrid generates correct number of LCHab colors", "[colorGrid]")
{
  std::array<double, 2> hue_lim{ 0, 360 };
  std::array<double, 2> chroma_lim{ 0, 100 };
  std::array<double, 2> light_lim{ 0, 100 };
  int n_points = 30;

  auto colors =
    colorGrid<colors::LCHab>(hue_lim, chroma_lim, light_lim, n_points);

  REQUIRE(colors.size() == n_points);
}

TEST_CASE("colorGrid values are within limits for HSL", "[colorGrid]")
{
  std::array<double, 2> hue_lim{ 10, 20 };
  std::array<double, 2> sat_lim{ 0.2, 0.8 };
  std::array<double, 2> light_lim{ 0.3, 0.7 };
  int n_points = 10;

  auto colors = colorGrid<colors::HSL>(hue_lim, sat_lim, light_lim, n_points);

  for (const auto& c : colors) {
    REQUIRE(c.h() >= hue_lim[0]);
    REQUIRE(c.h() <= hue_lim[1]);
    REQUIRE(c.s() >= sat_lim[0]);
    REQUIRE(c.s() <= sat_lim[1]);
    REQUIRE(c.l() >= light_lim[0]);
    REQUIRE(c.l() <= light_lim[1]);
  }
}
