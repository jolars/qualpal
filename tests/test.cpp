#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal/colors.h>

TEST_CASE("Converting between colors work", "[factorial]")
{
  using namespace Catch::Matchers;

  qualpal::RGB rgb(0.23, 0.5, 0.95);
  qualpal::XYZ xyz(rgb);

  REQUIRE_THAT(xyz.x_value(), WithinAbsMatcher(0.254958, 0.001));
  REQUIRE_THAT(xyz.y_value(), WithinAbsMatcher(0.226503, 0.001));
  REQUIRE_THAT(xyz.z_value(), WithinAbsMatcher(0.872124, 0.001));
}
