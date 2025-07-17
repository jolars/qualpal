#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <qualpal/analyze.h>
#include <qualpal/colors.h>
#include <string>
#include <vector>

using qualpal::analyzePalette;
using qualpal::PaletteAnalysisMap;

TEST_CASE("analyzePalette returns results for all default CVD types",
          "[analyze]")
{
  std::vector<qualpal::colors::RGB> palette = {
    { 1.0, 0.0, 0.0 }, // Red
    { 0.0, 1.0, 0.0 }, // Green
    { 0.0, 0.0, 1.0 }  // Blue
  };

  PaletteAnalysisMap result = analyzePalette(palette);

  REQUIRE(result.count("normal") == 1);
  REQUIRE(result.count("deutan") == 1);
  REQUIRE(result.count("protan") == 1);
  REQUIRE(result.count("tritan") == 1);

  for (const auto& [cvd_type, analysis] : result) {
    REQUIRE(analysis.difference_matrix.nrow() == 3);
    REQUIRE(analysis.difference_matrix.ncol() == 3);
    REQUIRE(analysis.min_distances.size() == 3);
    // No background, so bg_min_distance should be NaN
    REQUIRE(std::isnan(analysis.bg_min_distance));
  }
}

TEST_CASE("analyzePalette supports custom CVD configuration and background",
          "[analyze]")
{
  std::vector<qualpal::colors::RGB> palette = {
    { 1.0, 1.0, 0.0 }, // Yellow
    { 0.0, 1.0, 1.0 }  // Cyan
  };

  std::map<std::string, double> cvd = { { "normal", 0.0 }, { "deutan", 0.5 } };
  qualpal::colors::RGB bg{ 0.0, 0.0, 0.0 }; // Black background

  PaletteAnalysisMap result =
    analyzePalette(palette, qualpal::metrics::MetricType::DIN99d, cvd, bg);

  REQUIRE(result.count("normal") == 1);
  REQUIRE(result.count("deutan") == 1);
  REQUIRE(result.count("protan") == 0);
  REQUIRE(result.count("tritan") == 0);

  for (const auto& [cvd_type, analysis] : result) {
    REQUIRE(analysis.difference_matrix.nrow() == 2);
    REQUIRE(analysis.difference_matrix.ncol() == 2);
    REQUIRE(analysis.min_distances.size() == 2);
    // With background, bg_min_distance should be a real value (not NaN)
    REQUIRE_FALSE(std::isnan(analysis.bg_min_distance));
    REQUIRE(analysis.bg_min_distance >= 0.0);
  }
}
