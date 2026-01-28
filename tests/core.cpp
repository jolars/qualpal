#include "../src/qualpal/color_grid.h"
#include "../src/qualpal/cvd.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal.h>

TEST_CASE("Running qualpal works", "[qualpal]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  std::vector<RGB> rgb_colors = {
    RGB(0.23, 0.5, 0.95), RGB(0.5, 0.23, 0.95), RGB(0.95, 0.23, 0.5),
    RGB(0.23, 0.95, 0.5), RGB(0.95, 0.5, 0.23), RGB(0.5, 0.95, 0.23),
  };

  qualpal::Qualpal qp1;
  qp1.setInputRGB(rgb_colors);
  std::vector<RGB> pal1 = qp1.generate(4);

  qualpal::Qualpal qp2;
  qp2.setInputColorspace({ 0, 360 }, { 1, 1 }, { 0, 1 });
  std::vector<RGB> pal2 = qp2.generate(4);
}

TEST_CASE("Color grid is correctly set up", "[color-grid]")
{
  using namespace qualpal::colors;

  std::vector<HSL> result =
    qualpal::colorGrid<HSL>({ -200, 120 }, { 0.3, 0.8 }, { 0.4, 0.9 }, 1000);

  for (auto& color : result) {
    HSL hsl = color;
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
  using namespace qualpal::colors;
  const double eps = 1e-6;

  auto result = qualpal::Qualpal{}
                  .setInputColorspace({ -200, 120 }, { 0.3, 0.8 }, { 0.4, 0.9 })
                  .generate(5);

  for (auto& color : result) {
    HSL hsl = color;
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
  using namespace qualpal::colors;

  qualpal::Qualpal qp;
  qp.setInputPalette("ColorBrewer:Set2");
  std::vector<RGB> result = qp.generate(2);

  REQUIRE(result[0].hex() == "#66c2a5");
  REQUIRE(result[1].hex() == "#fc8d62");

  REQUIRE_THROWS_AS(qp.setInputPalette("ColorBrewer:NonExistentPalette"),
                    std::invalid_argument);
}

TEST_CASE("Separated colors", "[colors]")
{
  using namespace qualpal::colors;

  RGB col1("#fe0000");
  RGB col2("#000000");
  RGB col3("#aa00ff");

  std::vector<RGB> colors = { col1, col2, col3 };
  auto result = qualpal::Qualpal{}.setInputRGB(colors).generate(2);

  REQUIRE(result[0].hex() != "#aa00ff");
  REQUIRE(result[1].hex() != "#aa00ff");
}

TEST_CASE("Background colors", "[colors]")
{
  using namespace qualpal::colors;

  RGB bg_color("#66c2a5");

  std::vector<RGB> result = qualpal::Qualpal{}
                              .setInputPalette("ColorBrewer:Set2")
                              .setBackground(bg_color)
                              .generate(2);

  REQUIRE(result[0].hex() != "#66c2a5");
}

TEST_CASE("Adapting to color vision deficiency", "[cvd]")
{
  using namespace qualpal;

  auto cvd = std::map<std::string, double>{
    { "deutan", 1.0 },
    { "protan", 1.0 },
    { "tritan", 1.0 },
  };

  std::vector<colors::RGB> result = qualpal::Qualpal{}
                                      .setInputPalette("ColorBrewer:Set2")
                                      .setCvd(cvd)
                                      .generate(2);

  SECTION("Erroneous CVD type")
  {
    std::map<std::string, double> invalid_cvd = { { "invalid", 1.0 } };
    REQUIRE_THROWS_AS(qualpal::Qualpal{}
                        .setInputPalette("ColorBrewer:Set2")
                        .setCvd(invalid_cvd)
                        .generate(2),
                      std::invalid_argument);
  }

  SECTION("CVD severity out of range")
  {
    std::map<std::string, double> invalid_cvd = { { "deutan", 1.5 } };
    REQUIRE_THROWS_AS(qualpal::Qualpal{}
                        .setInputPalette("ColorBrewer:Set2")
                        .setCvd(invalid_cvd)
                        .generate(2),
                      std::invalid_argument);
  }
}

TEST_CASE("Using different metrics", "[metrics][fail]")
{
  using namespace qualpal;

  auto qp =
    qualpal::Qualpal{}
      .setInputPalette("ColorBrewer:Set2")
      .setCvd({ { "deutan", 1.0 }, { "protan", 0.8 }, { "tritan", 1.0 } });

  REQUIRE_NOTHROW(qp.setMetric(metrics::MetricType::CIE76).generate(2));
  REQUIRE_NOTHROW(qp.setMetric(metrics::MetricType::CIEDE2000).generate(2));
}

TEST_CASE("Qualpal::extend preserves fixed palette and adds distinct colors",
          "[qualpal][extend]")
{
  using namespace qualpal;

  SECTION("Extend to five colors")
  {
    Qualpal qp;
    std::vector<colors::RGB> input = {
      colors::RGB("#ff0000"), colors::RGB("#00ff00"), colors::RGB("#0000ff"),
      colors::RGB("#ffff00"), colors::RGB("#00ffff"), colors::RGB("#ff00ff"),
      colors::RGB("#ffffff"), colors::RGB("#000000")
    };
    qp.setInputRGB(input);

    // Fixed palette: first 3 colors
    std::vector<colors::RGB> fixed = { colors::RGB("#ff0000"),
                                       colors::RGB("#00ff00"),
                                       colors::RGB("#0000ff") };

    // Extend to 5 colors
    auto extended = qp.extend(fixed, 5);

    REQUIRE(extended.size() == 5);
    REQUIRE(extended[0] == fixed[0]);
    REQUIRE(extended[1] == fixed[1]);
    REQUIRE(extended[2] == fixed[2]);

    // The remaining colors should be from input but not in fixed
    std::vector<colors::RGB> remaining;
    for (const auto& c : input) {
      if (std::find(fixed.begin(), fixed.end(), c) == fixed.end())
        remaining.push_back(c);
    }

    REQUIRE(std::find(remaining.begin(), remaining.end(), extended[3]) !=
            remaining.end());
    REQUIRE(std::find(remaining.begin(), remaining.end(), extended[4]) !=
            remaining.end());
    REQUIRE(extended[3] != extended[4]);
  }

  SECTION("extend: palette is extended with new distinct colors")
  {
    qualpal::Qualpal qp;
    std::vector<qualpal::colors::RGB> input = {
      qualpal::colors::RGB("#ff0000"), qualpal::colors::RGB("#00ff00"),
      qualpal::colors::RGB("#0000ff"), qualpal::colors::RGB("#ffff00"),
      qualpal::colors::RGB("#00ffff"), qualpal::colors::RGB("#ff00ff"),
    };
    qp.setInputRGB(input);

    std::vector<qualpal::colors::RGB> fixed = {
      qualpal::colors::RGB("#ff0000"),
      qualpal::colors::RGB("#00ff00"),
    };

    auto extended = qp.extend(fixed, 4);

    REQUIRE(extended.size() == 4);
    REQUIRE(extended[0] == fixed[0]);
    REQUIRE(extended[1] == fixed[1]);
    // The remaining colors should be from input but not in fixed
    std::vector<qualpal::colors::RGB> remaining;
    for (const auto& c : input) {
      if (std::find(fixed.begin(), fixed.end(), c) == fixed.end())
        remaining.push_back(c);
    }
    REQUIRE(std::find(remaining.begin(), remaining.end(), extended[2]) !=
            remaining.end());
    REQUIRE(std::find(remaining.begin(), remaining.end(), extended[3]) !=
            remaining.end());
    REQUIRE(extended[2] != extended[3]);
  }

  SECTION("extend: throws if n < fixed.size()")
  {
    qualpal::Qualpal qp;
    std::vector<qualpal::colors::RGB> input = {
      qualpal::colors::RGB("#ff0000"),
      qualpal::colors::RGB("#00ff00"),
      qualpal::colors::RGB("#0000ff"),
    };
    qp.setInputRGB(input);

    std::vector<qualpal::colors::RGB> fixed = {
      qualpal::colors::RGB("#ff0000"),
      qualpal::colors::RGB("#00ff00"),
    };

    REQUIRE_THROWS_AS(qp.extend(fixed, 1), std::invalid_argument);
  }
}

TEST_CASE("Qualpal::extend - Basic functionality")
{
  qualpal::Qualpal qp;

  // Input fixed palette
  std::vector<qualpal::colors::RGB> fixed_palette = {
    { 1, 0, 0 }, // Red
    { 0, 1, 0 }  // Green
  };

  // Input candidate colors
  std::vector<qualpal::colors::RGB> candidates = {
    { 0, 0, 1 }, // Blue
    { 1, 1, 0 }, // Yellow
    { 0, 1, 1 }  // Cyan
  };

  qp.setInputRGB(candidates);

  SECTION("Extend palette to 4 colors")
  {
    auto extended_palette = qp.extend(fixed_palette, 4);

    REQUIRE(extended_palette.size() == 4);
    REQUIRE(extended_palette[0] == fixed_palette[0]); // Red
    REQUIRE(extended_palette[1] == fixed_palette[1]); // Green
  }

  SECTION("Extend palette to 5 colors")
  {
    auto extended_palette = qp.extend(fixed_palette, 5);

    REQUIRE(extended_palette.size() == 5);
    REQUIRE(extended_palette[0] == fixed_palette[0]); // Red
    REQUIRE(extended_palette[1] == fixed_palette[1]); // Green
  }
}

TEST_CASE("Qualpal::extend - Edge cases")
{
  qualpal::Qualpal qp;

  // Input candidate colors
  std::vector<qualpal::colors::RGB> candidates = {
    { 0, 0, 1 }, // Blue
    { 1, 1, 0 }, // Yellow
    { 0, 1, 1 }  // Cyan
  };

  qp.setInputRGB(candidates);

  SECTION("Empty fixed palette")
  {
    std::vector<qualpal::colors::RGB> fixed_palette = {};
    auto extended_palette = qp.extend(fixed_palette, 3);

    REQUIRE(extended_palette.size() == 3);
  }

  SECTION("Fixed palette with one color")
  {
    std::vector<qualpal::colors::RGB> fixed_palette = {
      { 1, 0, 0 } // Red
    };
    auto extended_palette = qp.extend(fixed_palette, 4);

    REQUIRE(extended_palette.size() == 4);
    REQUIRE(extended_palette[0] == fixed_palette[0]); // Red
  }

  SECTION("Fixed palette with all candidate colors")
  {
    std::vector<qualpal::colors::RGB> fixed_palette = candidates;
    auto extended_palette = qp.extend(fixed_palette, 3);

    REQUIRE(extended_palette.size() == 3);
    REQUIRE(extended_palette == fixed_palette);
  }
}

TEST_CASE("Qualpal::extend - Regression test with fixed seed")
{
  using namespace Catch::Matchers;

  qualpal::Qualpal qp;

  // Input candidate colors
  qp.setInputColorspace({ 0, 360 }, { 0.3, 0.8 }, { 0.4, 0.9 });

  // Fixed palette
  std::vector<qualpal::colors::RGB> fixed_palette = {
    { 1, 0, 0 }, // Red
    { 0, 1, 0 }  // Green
  };

  SECTION("Extend palette to 4 colors")
  {
    auto extended_palette = qp.extend(fixed_palette, 4);

    REQUIRE(extended_palette.size() == 4);
    REQUIRE(extended_palette[0] == fixed_palette[0]); // Red
    REQUIRE(extended_palette[1] == fixed_palette[1]); // Green

    REQUIRE_THAT(extended_palette[2].r(), WithinAbs(0.1151009398148148, 1e-5));
    REQUIRE_THAT(extended_palette[2].g(), WithinAbs(0.10663743209876542, 1e-5));
    REQUIRE_THAT(extended_palette[2].b(), WithinAbs(0.72568256790123464, 1e-5));
  }
}

TEST_CASE("Multiple colorspace regions work", "[colorspace-regions]")
{
  using namespace qualpal::colors;
  const double eps = 1e-6;

  SECTION("Two disjoint regions in HSL")
  {
    // Warm colors (reds/oranges) and cool colors (blues/cyans)
    auto result = qualpal::Qualpal{}
                    .setInputColorspaceRegions(
                      {
                        { { 0, 60 }, { 0.5, 1.0 }, { 0.3, 0.7 } },
                        { { 180, 240 }, { 0.5, 1.0 }, { 0.3, 0.7 } },
                      },
                      qualpal::ColorspaceType::HSL)
                    .generate(6);

    REQUIRE(result.size() == 6);

    // All colors should be in the specified ranges
    for (const auto& color : result) {
      HSL hsl = color;
      bool in_warm = (hsl.h() >= 0 - eps && hsl.h() <= 60 + eps);
      bool in_cool = (hsl.h() >= 180 - eps && hsl.h() <= 240 + eps);
      REQUIRE((in_warm || in_cool));
      REQUIRE(hsl.s() >= 0.5 - eps);
      REQUIRE(hsl.s() <= 1.0 + eps);
      REQUIRE(hsl.l() >= 0.3 - eps);
      REQUIRE(hsl.l() <= 0.7 + eps);
    }
  }

  SECTION("Three regions with different lightness")
  {
    // Dark, medium, and light colors
    auto result = qualpal::Qualpal{}
                    .setInputColorspaceRegions(
                      {
                        { { 0, 360 }, { 0.6, 1.0 }, { 0.1, 0.3 } },
                        { { 0, 360 }, { 0.6, 1.0 }, { 0.4, 0.6 } },
                        { { 0, 360 }, { 0.6, 1.0 }, { 0.7, 0.9 } },
                      },
                      qualpal::ColorspaceType::HSL)
                    .generate(9);

    REQUIRE(result.size() == 9);

    for (const auto& color : result) {
      HSL hsl = color;
      bool in_dark = (hsl.l() >= 0.1 - eps && hsl.l() <= 0.3 + eps);
      bool in_medium = (hsl.l() >= 0.4 - eps && hsl.l() <= 0.6 + eps);
      bool in_light = (hsl.l() >= 0.7 - eps && hsl.l() <= 0.9 + eps);
      REQUIRE((in_dark || in_medium || in_light));
    }
  }

  SECTION("Overlapping regions in LCHab")
  {
    // Two overlapping regions to emphasize certain hue range
    auto result = qualpal::Qualpal{}
                    .setInputColorspaceRegions(
                      {
                        { { 0, 120 }, { 30, 60 }, { 40, 70 } },
                        { { 60, 180 }, { 30, 60 }, { 40, 70 } },
                      },
                      qualpal::ColorspaceType::LCHab)
                    .generate(5);

    REQUIRE(result.size() == 5);

    // All colors should be within the union of the regions
    for (const auto& color : result) {
      LCHab lch = color;
      REQUIRE(lch.h() >= 0 - eps);
      REQUIRE(lch.h() <= 180 + eps);
      REQUIRE(lch.c() >= 30 - eps);
      REQUIRE(lch.c() <= 60 + eps);
      REQUIRE(lch.l() >= 40 - eps);
      REQUIRE(lch.l() <= 70 + eps);
    }
  }

  SECTION("Single region via setInputColorspaceRegions")
  {
    // Should work the same as setInputColorspace
    auto result = qualpal::Qualpal{}
                    .setInputColorspaceRegions(
                      {
                        { { 0, 360 }, { 0.3, 0.8 }, { 0.4, 0.9 } },
                      },
                      qualpal::ColorspaceType::HSL)
                    .generate(5);

    REQUIRE(result.size() == 5);

    for (const auto& color : result) {
      HSL hsl = color;
      REQUIRE(hsl.s() >= 0.3 - eps);
      REQUIRE(hsl.s() <= 0.8 + eps);
      REQUIRE(hsl.l() >= 0.4 - eps);
      REQUIRE(hsl.l() <= 0.9 + eps);
    }
  }

  SECTION("Empty regions vector throws")
  {
    qualpal::Qualpal qp;
    REQUIRE_THROWS_AS(
      qp.setInputColorspaceRegions({}, qualpal::ColorspaceType::HSL),
      std::invalid_argument);
  }

  SECTION("Invalid ranges in region throw")
  {
    qualpal::Qualpal qp;
    // Saturation out of range for HSL
    REQUIRE_THROWS_AS(qp.setInputColorspaceRegions(
                        {
                          { { 0, 360 }, { -0.1, 0.8 }, { 0.4, 0.9 } },
                        },
                        qualpal::ColorspaceType::HSL),
                      std::invalid_argument);
  }
}

TEST_CASE("setInputColorspace delegates to setInputColorspaceRegions",
          "[colorspace-regions]")
{
  using namespace qualpal::colors;
  const double eps = 1e-6;

  // Both methods should produce the same result
  auto result1 = qualpal::Qualpal{}
                   .setInputColorspace({ 0, 180 }, { 0.5, 1.0 }, { 0.3, 0.7 })
                   .setColorspaceSize(500)
                   .generate(5);

  auto result2 = qualpal::Qualpal{}
                   .setInputColorspaceRegions(
                     {
                       { { 0, 180 }, { 0.5, 1.0 }, { 0.3, 0.7 } },
                     },
                     qualpal::ColorspaceType::HSL)
                   .setColorspaceSize(500)
                   .generate(5);

  REQUIRE(result1.size() == 5);
  REQUIRE(result2.size() == 5);

  // Both should respect the same constraints
  for (const auto& color : result1) {
    HSL hsl = color;
    REQUIRE(hsl.h() >= 0 - eps);
    REQUIRE(hsl.h() <= 180 + eps);
  }

  for (const auto& color : result2) {
    HSL hsl = color;
    REQUIRE(hsl.h() >= 0 - eps);
    REQUIRE(hsl.h() <= 180 + eps);
  }
}

TEST_CASE("CVD simulations are applied independently", "[cvd][qualpal]")
{
  using namespace qualpal;
  using namespace qualpal::colors;
  using namespace Catch::Matchers;

  SECTION("Multiple CVD types produce order-independent results")
  {
    // Create a palette with multiple CVD constraints
    std::vector<RGB> input = {
      RGB(1.0, 0.0, 0.0), // Red
      RGB(1.0, 0.5, 0.0), // Orange
      RGB(0.0, 1.0, 0.0), // Green
      RGB(0.0, 0.0, 1.0), // Blue
      RGB(1.0, 1.0, 0.0), // Yellow
      RGB(0.0, 1.0, 1.0), // Cyan
    };

    // Test with protan + deutan
    Qualpal qp1;
    qp1.setInputRGB(input);
    qp1.setCvd({ { "protan", 1.0 }, { "deutan", 1.0 } });
    auto palette1 = qp1.generate(3);

    // Test with deutan + protan (different order)
    Qualpal qp2;
    qp2.setInputRGB(input);
    qp2.setCvd({ { "deutan", 1.0 }, { "protan", 1.0 } });
    auto palette2 = qp2.generate(3);

    // Results should be identical (order-independent)
    REQUIRE(palette1.size() == 3);
    REQUIRE(palette2.size() == 3);
    for (size_t i = 0; i < 3; ++i) {
      REQUIRE_THAT(palette1[i].r(), WithinAbs(palette2[i].r(), 1e-10));
      REQUIRE_THAT(palette1[i].g(), WithinAbs(palette2[i].g(), 1e-10));
      REQUIRE_THAT(palette1[i].b(), WithinAbs(palette2[i].b(), 1e-10));
    }
  }

  SECTION("CVD constraints improve palette distinguishability for CVD viewers")
  {
    // Red and orange are close under protan simulation
    std::vector<RGB> input = {
      RGB(1.0, 0.0, 0.0), // Red
      RGB(1.0, 0.5, 0.0), // Orange
      RGB(0.0, 0.0, 1.0), // Blue
      RGB(0.0, 1.0, 1.0), // Cyan
    };

    // Without CVD constraints - might select red and orange
    Qualpal qp_no_cvd;
    qp_no_cvd.setInputRGB(input);
    auto palette_no_cvd = qp_no_cvd.generate(2);

    // With protan constraints - should prefer more distinct colors
    Qualpal qp_with_cvd;
    qp_with_cvd.setInputRGB(input);
    qp_with_cvd.setCvd({ { "protan", 1.0 } });
    auto palette_with_cvd = qp_with_cvd.generate(2);

    REQUIRE(palette_no_cvd.size() == 2);
    REQUIRE(palette_with_cvd.size() == 2);

    // Verify that CVD-aware palette maintains distinguishability under protan
    auto color1_cvd = simulateCvd(palette_with_cvd[0], "protan", 1.0);
    auto color2_cvd = simulateCvd(palette_with_cvd[1], "protan", 1.0);

    // Colors should be sufficiently different under CVD simulation
    double diff_r = std::abs(color1_cvd.r() - color2_cvd.r());
    double diff_g = std::abs(color1_cvd.g() - color2_cvd.g());
    double diff_b = std::abs(color1_cvd.b() - color2_cvd.b());
    double total_diff = diff_r + diff_g + diff_b;

    // Should have some meaningful difference (not collapsed to same color)
    REQUIRE(total_diff > 0.1);
  }

  SECTION("Empty CVD map behaves like no CVD constraints")
  {
    std::vector<RGB> input = {
      RGB(1.0, 0.0, 0.0),
      RGB(0.0, 1.0, 0.0),
      RGB(0.0, 0.0, 1.0),
      RGB(1.0, 1.0, 0.0),
    };

    Qualpal qp1;
    qp1.setInputRGB(input);
    auto palette1 = qp1.generate(2);

    Qualpal qp2;
    qp2.setInputRGB(input);
    qp2.setCvd({});
    auto palette2 = qp2.generate(2);

    REQUIRE(palette1.size() == 2);
    REQUIRE(palette2.size() == 2);

    // Should produce identical results
    for (size_t i = 0; i < 2; ++i) {
      REQUIRE_THAT(palette1[i].r(), WithinAbs(palette2[i].r(), 1e-10));
      REQUIRE_THAT(palette1[i].g(), WithinAbs(palette2[i].g(), 1e-10));
      REQUIRE_THAT(palette1[i].b(), WithinAbs(palette2[i].b(), 1e-10));
    }
  }

  SECTION("CVD severity of 0 is equivalent to no CVD")
  {
    std::vector<RGB> input = {
      RGB(1.0, 0.0, 0.0),
      RGB(0.0, 1.0, 0.0),
      RGB(0.0, 0.0, 1.0),
    };

    Qualpal qp1;
    qp1.setInputRGB(input);
    auto palette1 = qp1.generate(2);

    Qualpal qp2;
    qp2.setInputRGB(input);
    qp2.setCvd({ { "protan", 0.0 }, { "deutan", 0.0 } });
    auto palette2 = qp2.generate(2);

    REQUIRE(palette1.size() == 2);
    REQUIRE(palette2.size() == 2);

    // Should produce identical results
    for (size_t i = 0; i < 2; ++i) {
      REQUIRE_THAT(palette1[i].r(), WithinAbs(palette2[i].r(), 1e-10));
      REQUIRE_THAT(palette1[i].g(), WithinAbs(palette2[i].g(), 1e-10));
      REQUIRE_THAT(palette1[i].b(), WithinAbs(palette2[i].b(), 1e-10));
    }
  }
}
