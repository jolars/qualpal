#include "../src/qualpal/color_grid.h"
#include "../src/qualpal/colors.h"
#include "../src/qualpal/cvd_simulation.h"
#include "../src/qualpal/matrix.h"
#include "../src/qualpal/qualpal.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("All coolor conversions are supported", "[colors]")
{
  using namespace Catch::Matchers;
  using namespace qualpal;

  XYZ xyz(0.4, 0.2, 0.65);
  RGB rgb(0.23, 0.5, 0.95);
  HSL hsl(190, 0.5, 1.0);
  Lab lab(0.3, 0.98, 0.25);
  DIN99d din99d(0.3, 0.98, 0.25);

  rgb = xyz;
  hsl = xyz;
  lab = xyz;
  din99d = xyz;

  REQUIRE_THAT(rgb.r(), WithinAbsMatcher(0.834916, 0.001));
  REQUIRE_THAT(rgb.g(), WithinAbsMatcher(0.125819, 0.001));
  REQUIRE_THAT(rgb.b(), WithinAbsMatcher(0.837110, 0.001));

  REQUIRE_THAT(lab.l(), WithinAbsMatcher(51.8372, 0.001));
  REQUIRE_THAT(lab.a(), WithinAbsMatcher(82.2926, 0.001));
  REQUIRE_THAT(lab.b(), WithinAbsMatcher(-51.4415, 0.001));

  rgb = hsl;
  xyz = hsl;
  lab = hsl;
  din99d = hsl;

  rgb = lab;
  xyz = lab;
  lab = lab;
  din99d = lab;

  REQUIRE_THAT(rgb.r(), WithinAbsMatcher(0.834916, 0.001));
  REQUIRE_THAT(rgb.g(), WithinAbsMatcher(0.125819, 0.001));
  REQUIRE_THAT(rgb.b(), WithinAbsMatcher(0.837110, 0.001));

  REQUIRE_THAT(xyz.x(), WithinAbsMatcher(0.400000, 0.001));
  REQUIRE_THAT(xyz.y(), WithinAbsMatcher(0.200000, 0.001));
  REQUIRE_THAT(xyz.z(), WithinAbsMatcher(0.650000, 0.001));
}

TEST_CASE("Converting between colors work", "[colors]")
{
  using namespace Catch::Matchers;

  qualpal::RGB rgb(0.23, 0.5, 0.95);
  qualpal::XYZ xyz(rgb);
  qualpal::HSL hsl(rgb);
  qualpal::DIN99d din99d(xyz);
  qualpal::Lab lab(xyz);

  REQUIRE_THAT(xyz.x(), WithinAbsMatcher(0.254958, 0.001));
  REQUIRE_THAT(xyz.y(), WithinAbsMatcher(0.226503, 0.001));
  REQUIRE_THAT(xyz.z(), WithinAbsMatcher(0.872124, 0.001));

  REQUIRE_THAT(lab.l(), WithinAbsMatcher(54.7103, 0.001));
  REQUIRE_THAT(lab.a(), WithinAbsMatcher(17.6776, 0.001));
  REQUIRE_THAT(lab.b(), WithinAbsMatcher(-63.8246, 0.001));

  REQUIRE_THAT(hsl.h(), WithinAbsMatcher(217.5, 0.001));
  REQUIRE_THAT(hsl.s(), WithinAbsMatcher(0.87805, 0.001));
  REQUIRE_THAT(hsl.l(), WithinAbsMatcher(0.59, 0.001));

  qualpal::RGB rgb_from_hex("#3b80f2");

  REQUIRE_THAT(rgb_from_hex.r(), WithinAbsMatcher(0.231372, 0.001));
  REQUIRE_THAT(rgb_from_hex.g(), WithinAbsMatcher(0.501960, 0.001));
  REQUIRE_THAT(rgb_from_hex.b(), WithinAbsMatcher(0.949019, 0.001));
}

TEST_CASE("Check that converted HSL colors are within correct ranges",
          "[colors]")
{
  using namespace Catch::Matchers;

  qualpal::RGB rgb(0.8109190, 0.4385978, 0.4837288);
  qualpal::HSL hsl(rgb);

  REQUIRE(hsl.h() >= 0);
  REQUIRE(hsl.h() <= 360);
  REQUIRE(hsl.s() >= 0);
  REQUIRE(hsl.s() <= 1);
  REQUIRE(hsl.l() >= 0);
  REQUIRE(hsl.l() <= 1);
}

TEST_CASE("Running qualpal works", "[colors]")
{
  using namespace Catch::Matchers;

  std::vector<qualpal::RGB> rgb_colors = {
    qualpal::RGB(0.23, 0.5, 0.95), qualpal::RGB(0.5, 0.23, 0.95),
    qualpal::RGB(0.95, 0.23, 0.5), qualpal::RGB(0.23, 0.95, 0.5),
    qualpal::RGB(0.95, 0.5, 0.23), qualpal::RGB(0.5, 0.95, 0.23),
  };

  std::vector<qualpal::RGB> pal1 = qualpal::qualpal(4, rgb_colors);

  std::vector<qualpal::RGB> pal2 =
    qualpal::qualpal(4, { 0, 360 }, { 1, 1 }, { 0, 1 });
}

TEST_CASE("Colors returned by qualpal are in legal ranges", "[colors]")
{
  using namespace Catch::Matchers;

  // auto res = qualpal(n)

  // REQUIRE_THAT(rgb_from_hex.r(), WithinAbsMatcher(0.231372, 0.001));
  // REQUIRE_THAT(rgb_from_hex.g(), WithinAbsMatcher(0.501960, 0.001));
  // REQUIRE_THAT(rgb_from_hex.b(), WithinAbsMatcher(0.949019, 0.001));
}

TEST_CASE("Matrix-Matrix multiplication", "[FixedMatrix]")
{
  qualpal::FixedMatrix<int, 2, 3> A = { { 1, 2, 3 }, { 4, 5, 6 } };
  qualpal::FixedMatrix<int, 3, 2> B = { { 7, 8 }, { 9, 10 }, { 11, 12 } };

  qualpal::FixedMatrix<int, 2, 2> result = A * B;

  REQUIRE(result(0, 0) == 58);
  REQUIRE(result(0, 1) == 64);
  REQUIRE(result(1, 0) == 139);
  REQUIRE(result(1, 1) == 154);
}

TEST_CASE("Matrix-MatrixTranspose multiplication", "[FixedMatrix]")
{
  qualpal::FixedMatrix<int, 2, 3> A = { { 1, 2, 3 }, { 4, 5, 6 } };
  qualpal::FixedMatrix<int, 2, 3> B = { { 7, 8, 9 }, { 10, 11, 12 } };

  qualpal::FixedMatrix<int, 2, 2> result = A * B.t();

  REQUIRE(result(0, 0) == 50);
  REQUIRE(result(0, 1) == 68);
  REQUIRE(result(1, 0) == 122);
  REQUIRE(result(1, 1) == 167);
}

TEST_CASE("CVD simulation", "[colors]")
{
  std::vector<qualpal::RGB> rgb = { qualpal::RGB(0.8, 0.4, 0.1),
                                    qualpal::RGB(0.1, 0.1, 0.1) };
  auto result = qualpal::simulate_cvd(rgb, "protan", 0.5);
  REQUIRE(result.size() == 2);
}

TEST_CASE("Color grid is correctly set up", "[color-grid]")
{
  std::vector<qualpal::HSL> result =
    qualpal::colorGrid({ -200, 120 }, { 0.3, 0.8 }, { 0.4, 0.9 }, 1000);

  for (auto& color : result) {
    qualpal::HSL hsl = color;
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
  const double eps = 1e-6;

  std::vector<qualpal::RGB> result =
    qualpal::qualpal(5, { -200, 120 }, { 0.3, 0.8 }, { 0.4, 0.9 });

  for (auto& color : result) {
    qualpal::HSL hsl = color;
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
  const double eps = 1e-6;
  std::vector<qualpal::RGB> result = qualpal::qualpal(2, "ColorBrewer:Set2");

  REQUIRE(result[0].hex() == "#66c2a5");
  REQUIRE(result[1].hex() == "#fc8d62");

  REQUIRE_THROWS(qualpal::qualpal(2, "awtools:non_existent_palette"));
  REQUIRE_THROWS_AS(qualpal::qualpal(100, "awtools:a_palette"),
                    std::invalid_argument);
}
