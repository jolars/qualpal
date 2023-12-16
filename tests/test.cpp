#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal/colors.h>
#include <qualpal/qualpal.h>

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
