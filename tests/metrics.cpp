#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal/colors.h>
#include <qualpal/metrics.h>

TEST_CASE("CIEDE2000 metric", "[metrics][ciede2000]")
{
  using namespace qualpal;
  using namespace qualpal::colors;
  using namespace Catch::Matchers;

  Lab c1(50, 2, 1);
  Lab c2(60, 3, 2);

  metrics::CIEDE2000 met;

  // References come from Bruce Lindbloom's CIEDE2000 calculator
  SECTION("Known values")
  {
    double diff = met(c1, c2);
    REQUIRE_THAT(diff, WithinAbs(9.593947, 1e-5));

    Lab c3(0, 2, 1);
    Lab c4(60, -98, 5);
    diff = met(c3, c4);

    REQUIRE_THAT(diff, WithinAbs(57.953547, 1e-5));

    Lab c5(90, -98, -50);
    Lab c6(100, -98, 5);

    diff = met(c5, c6);

    REQUIRE_THAT(diff, WithinAbs(22.532797, 1e-5));
  }

  SECTION("Symmetricity")
  {
    double diff1 = met(c1, c2);
    double diff2 = met(c2, c1);
    REQUIRE_THAT(diff1, WithinAbs(diff2, 1e-5));
  }

  SECTION("Identity")
  {
    double diff = met(c1, c1);
    REQUIRE_THAT(diff, WithinAbs(0, 1e-5));
  }
}

TEST_CASE("DIN99d metric", "[metrics][din99d]")
{
  using namespace qualpal;
  using namespace qualpal::colors;
  using namespace Catch::Matchers;

  DIN99d c1(10, 2, 1);
  DIN99d c2(60, 3, 2);

  metrics::DIN99d met;

  SECTION("Known values")
  {
    double diff = met(c1, c2);
    REQUIRE_THAT(diff, WithinAbs(23.15134744049614923, 1e-5));
  }

  SECTION("Symmetricity")
  {
    double diff1 = met(c1, c2);
    double diff2 = met(c2, c1);
    REQUIRE_THAT(diff1, WithinAbs(diff2, 1e-5));
  }
}

TEST_CASE("CIE76 metric", "[metrics][cie76]")
{
  using namespace qualpal;
  using namespace qualpal::colors;
  using namespace Catch::Matchers;

  Lab c1(10, 2, 1);
  Lab c2(60, 3, 2);

  metrics::CIE76 met;

  SECTION("Known values")
  {
    double diff = met(c1, c2);
    REQUIRE_THAT(diff, WithinAbs(50.019996, 1e-5));
  }
}
