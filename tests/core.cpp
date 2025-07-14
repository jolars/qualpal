#include "../src/qualpal/color_grid.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal.h>

TEST_CASE("Running qualpal works", "[colors]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  std::vector<RGB> rgb_colors = {
    RGB(0.23, 0.5, 0.95), RGB(0.5, 0.23, 0.95), RGB(0.95, 0.23, 0.5),
    RGB(0.23, 0.95, 0.5), RGB(0.95, 0.5, 0.23), RGB(0.5, 0.95, 0.23),
  };

  std::vector<RGB> pal1 = qualpal::qualpal(4, rgb_colors);

  std::vector<RGB> pal2 = qualpal::qualpal(4, { 0, 360 }, { 1, 1 }, { 0, 1 });
}

TEST_CASE("Color grid is correctly set up", "[color-grid]")
{
  using namespace qualpal::colors;

  std::vector<HSL> result =
    qualpal::colorGrid({ -200, 120 }, { 0.3, 0.8 }, { 0.4, 0.9 }, 1000);

  for (auto& color : result) {
    HSL hsl = color;
    REQUIRE(hsl.h() >= 0);
    REQUIRE(hsl.h() <= 360);
    REQUIRE((hsl.h() <= 120 || hsl.h() >= 160));
    REQUIRE(hsl.s() >= 0.3);
    REQUIRE(hsl.s() <= 0.8);
    REQUIRE(hsl.l() >= 0.4);
    REQUIRE(hsl.l() <= 0.9);
  }

  REQUIRE(result.size() == 1000);
}

TEST_CASE("Output within ranges for colorspace method", "[colors]")
{
  using namespace qualpal::colors;
  const double eps = 1e-6;

  std::vector<RGB> result =
    qualpal::qualpal(5, { -200, 120 }, { 0.3, 0.8 }, { 0.4, 0.9 });

  for (auto& color : result) {
    HSL hsl = color;
    REQUIRE(hsl.h() >= 0 - eps);
    REQUIRE(hsl.h() <= 360 + eps);
    REQUIRE((hsl.h() <= 120 + eps || hsl.h() >= 160 - eps));
    REQUIRE(hsl.s() >= 0.3 - eps);
    REQUIRE(hsl.s() <= 0.8 + eps);
    REQUIRE(hsl.l() >= 0.4 - eps);
    REQUIRE(hsl.l() <= 0.9 + eps);
  }
}

TEST_CASE("Character method works as it it supposed to", "[colors]")
{
  using namespace qualpal::colors;

  std::vector<RGB> result = qualpal::qualpal(2, "ColorBrewer:Set2");

  REQUIRE(result[0].hex() == "#a6d854");
  REQUIRE(result[1].hex() == "#e78ac3");

  REQUIRE_THROWS(qualpal::qualpal(2, "awtools:non_existent_palette"));
  REQUIRE_THROWS_AS(qualpal::qualpal(100, "awtools:a_palette"),
                    std::invalid_argument);
}

TEST_CASE("Separated colors", "[colors]")
{
  using namespace qualpal::colors;

  RGB col1("#fe0000");
  RGB col2("#000000");
  RGB col3("#aa00ff");

  std::vector<RGB> colors = { col1, col2, col3 };
  std::vector<RGB> result = qualpal::qualpal(2, colors);

  REQUIRE(result[0].hex() != "#aa00ff");
  REQUIRE(result[1].hex() != "#aa00ff");
}

TEST_CASE("Background colors", "[colors][fail]")
{
  using namespace qualpal::colors;

  const double eps = 1e-6;

  RGB bg_color("#66c2a5");

  std::vector<RGB> result =
    qualpal::qualpal(2, "ColorBrewer:Set2", {}, bg_color);
  REQUIRE(result[0].hex() != "#66c2a5");
}

TEST_CASE("Adapting to color vision deficiency", "[cvd][fail]")
{
  using namespace qualpal;

  const double eps = 1e-6;

  auto cvd = std::map<std::string, double>{
    { "deuter", 1.0 },
    { "protan", 1.0 },
    { "tritan", 1.0 },
  };

  std::vector<colors::RGB> result =
    qualpal::qualpal(2, "ColorBrewer:Set2", cvd);
}
