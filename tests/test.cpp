#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal/colors.h>

TEST_CASE("Converting between colors work", "[factorial]")
{
  using namespace Catch::Matchers;

  qualpal::RGB rgb(0.23, 0.5, 0.95);
  qualpal::XYZ xyz(rgb);
  qualpal::HSL hsl(rgb);

  REQUIRE_THAT(xyz.x(), WithinAbsMatcher(0.254958, 0.001));
  REQUIRE_THAT(xyz.y(), WithinAbsMatcher(0.226503, 0.001));
  REQUIRE_THAT(xyz.z(), WithinAbsMatcher(0.872124, 0.001));
}
