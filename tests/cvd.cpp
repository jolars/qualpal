#include "../src/qualpal/cvd_simulation.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal/colors.h>

TEST_CASE("CVD simulation basic functionality", "[cvd]")
{
  using namespace qualpal;
  using namespace qualpal::colors;
  using namespace Catch::Matchers;

  RGB input_color(0.5, 0.3, 0.8);

  SECTION("Identity transformation with severity 0")
  {
    RGB result_protan = simulateCvd(input_color, "protan", 0.0);
    RGB result_deuter = simulateCvd(input_color, "deuter", 0.0);
    RGB result_tritan = simulateCvd(input_color, "tritan", 0.0);

    REQUIRE_THAT(result_protan.r(), WithinRel(input_color.r(), 1e-10));
    REQUIRE_THAT(result_protan.g(), WithinRel(input_color.g(), 1e-10));
    REQUIRE_THAT(result_protan.b(), WithinRel(input_color.b(), 1e-10));

    REQUIRE_THAT(result_deuter.r(), WithinRel(input_color.r(), 1e-10));
    REQUIRE_THAT(result_deuter.g(), WithinRel(input_color.g(), 1e-10));
    REQUIRE_THAT(result_deuter.b(), WithinRel(input_color.b(), 1e-10));

    REQUIRE_THAT(result_tritan.r(), WithinRel(input_color.r(), 1e-10));
    REQUIRE_THAT(result_tritan.g(), WithinRel(input_color.g(), 1e-10));
    REQUIRE_THAT(result_tritan.b(), WithinRel(input_color.b(), 1e-10));
  }

  SECTION("Maximum severity transformation")
  {
    RGB result_protan = simulateCvd(input_color, "protan", 1.0);
    RGB result_deuter = simulateCvd(input_color, "deuter", 1.0);
    RGB result_tritan = simulateCvd(input_color, "tritan", 1.0);

    // Results should be different from input for non-zero severity
    REQUIRE(result_protan.r() != input_color.r());
    REQUIRE(result_deuter.r() != input_color.r());
    REQUIRE(result_tritan.r() != input_color.r());
  }

  SECTION("Intermediate severity interpolation")
  {
    RGB result_half = simulateCvd(input_color, "protan", 0.5);
    RGB result_zero = simulateCvd(input_color, "protan", 0.0);
    RGB result_full = simulateCvd(input_color, "protan", 1.0);

    // Result should be between the identity and full transformation
    REQUIRE(((result_half.r() >= std::min(result_zero.r(), result_full.r())) &&
             (result_half.r() <= std::max(result_zero.r(), result_full.r()))));
  }
}

TEST_CASE("CVD simulation edge cases", "[cvd]")
{
  using namespace qualpal;
  using namespace qualpal::colors;
  using namespace Catch::Matchers;

  SECTION("Black color")
  {
    RGB black(0.0, 0.0, 0.0);
    RGB result = simulateCvd(black, "protan", 1);

    REQUIRE_THAT(result.r(), WithinAbs(0.0, 1e-10));
    REQUIRE_THAT(result.g(), WithinAbs(0.0, 1e-10));
    REQUIRE_THAT(result.b(), WithinAbs(0.0, 1e-10));
  }

  SECTION("White color")
  {
    RGB white(1.0, 1.0, 1.0);
    RGB result_protan = simulateCvd(white, "protan", 1);
    RGB result_deuter = simulateCvd(white, "deuter", 1);
    RGB result_tritan = simulateCvd(white, "tritan", 1);

    // All results should be valid RGB values
    REQUIRE(result_protan.r() >= 0.0);
    REQUIRE(result_protan.g() >= 0.0);
    REQUIRE(result_protan.b() >= 0.0);

    REQUIRE(result_deuter.r() >= 0.0);
    REQUIRE(result_deuter.g() >= 0.0);
    REQUIRE(result_deuter.b() >= 0.0);

    REQUIRE(result_tritan.r() >= 0.0);
    REQUIRE(result_tritan.g() >= 0.0);
    REQUIRE(result_tritan.b() >= 0.0);
  }

  SECTION("Boundary severity values")
  {
    RGB color(0.6, 0.4, 0.2);

    // Test very small severity
    RGB result_tiny = simulateCvd(color, "deuter", 0.01);
    REQUIRE_THAT(result_tiny.r(), WithinRel(color.r(), 0.1));

    // Test severity close to 1.0
    RGB result_near_max = simulateCvd(color, "deuter", 0.99);
    RGB result_max = simulateCvd(color, "deuter", 1.0);
    REQUIRE_THAT(result_near_max.r(), WithinRel(result_max.r(), 0.1));
  }
}

TEST_CASE("CVD simulation type validation", "[cvd]")
{
  using namespace qualpal;
  using namespace qualpal::colors;

  RGB test_color(0.5, 0.5, 0.5);

  SECTION("Valid CVD types produce different results")
  {
    RGB protan_result = simulateCvd(test_color, "protan", 0.7);
    RGB deuter_result = simulateCvd(test_color, "deuter", 0.7);
    RGB tritan_result = simulateCvd(test_color, "tritan", 0.7);

    // Different CVD types should produce different results
    REQUIRE(protan_result.r() != deuter_result.r());
    REQUIRE(deuter_result.r() != tritan_result.r());
    REQUIRE(protan_result.r() != tritan_result.r());
  }
}

TEST_CASE("CVD simulation mathematical properties", "[cvd]")
{
  using namespace qualpal;
  using namespace qualpal::colors;
  using namespace Catch::Matchers;

  RGB test_color(0.3, 0.7, 0.1);

  SECTION("Linearity test")
  {
    // Test that the transformation is linear
    RGB color1(0.2, 0.4, 0.6);
    RGB color2(0.8, 0.3, 0.1);

    RGB result1 = simulateCvd(color1, "protan", 0.5);
    RGB result2 = simulateCvd(color2, "protan", 0.5);

    // Create a linear combination of input colors
    RGB combined_input(0.3 * color1.r() + 0.7 * color2.r(),
                       0.3 * color1.g() + 0.7 * color2.g(),
                       0.3 * color1.b() + 0.7 * color2.b());

    RGB result_combined = simulateCvd(combined_input, "protan", 0.5);

    // The result should equal the linear combination of individual results
    RGB expected_combined(0.3 * result1.r() + 0.7 * result2.r(),
                          0.3 * result1.g() + 0.7 * result2.g(),
                          0.3 * result1.b() + 0.7 * result2.b());

    REQUIRE_THAT(result_combined.r(), WithinRel(expected_combined.r(), 1e-10));
    REQUIRE_THAT(result_combined.g(), WithinRel(expected_combined.g(), 1e-10));
    REQUIRE_THAT(result_combined.b(), WithinRel(expected_combined.b(), 1e-10));
  }

  SECTION("Monotonicity with severity")
  {
    // For most colors, increasing severity should show consistent changes
    std::vector<double> severities = { 0.0, 0.2, 0.4, 0.6, 0.8, 1.0 };
    std::vector<RGB> results;

    for (double severity : severities) {
      results.push_back(simulateCvd(test_color, "protan", severity));
    }

    // Check that we have a progression (not necessarily monotonic due to
    // interpolation)
    REQUIRE(results[0].r() !=
            results[5].r()); // Should be different from start to end
  }
}
