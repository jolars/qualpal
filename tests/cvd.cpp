#include "../src/qualpal/cvd_simulation.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("CVD simulation", "[colors]")
{
  using namespace qualpal::colors;
  RGB rgb = RGB(0.8, 0.4, 0.1);
  REQUIRE_NOTHROW(qualpal::simulateCvd(rgb, "protan", 0.5));
}
