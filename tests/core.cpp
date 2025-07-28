#include "../src/qualpal/color_grid.h"
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

  REQUIRE(result[0].hex() == "#a6d854");
  REQUIRE(result[1].hex() == "#e78ac3");

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

    REQUIRE_THAT(extended_palette[2].r(), WithinAbs(0.21502551440329215, 1e-5));
    REQUIRE_THAT(extended_palette[2].g(), WithinAbs(0.22243096707818927, 1e-5));
    REQUIRE_THAT(extended_palette[2].b(), WithinAbs(0.68897448559670793, 1e-5));
  }
}
