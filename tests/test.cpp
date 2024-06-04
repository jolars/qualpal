#include "../src/qualpal/colors.h"
#include "../src/qualpal/matrix.h"
#include "../src/qualpal/qualpal.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

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

  qualpal::RGB rgb_from_hex("#3b80f2");

  REQUIRE_THAT(rgb_from_hex.r(), WithinAbsMatcher(0.231372, 0.001));
  REQUIRE_THAT(rgb_from_hex.g(), WithinAbsMatcher(0.501960, 0.001));
  REQUIRE_THAT(rgb_from_hex.b(), WithinAbsMatcher(0.949019, 0.001));
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
