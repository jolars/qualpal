#include "../src/qualpal/cvd_simulation.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("CVD simulation", "[colors]")
{
  std::vector<qualpal::RGB> rgb = { qualpal::RGB(0.8, 0.4, 0.1),
                                    qualpal::RGB(0.1, 0.1, 0.1) };
  auto result = qualpal::simulate_cvd(rgb, "protan", 0.5);
  REQUIRE(result.size() == 2);
}
