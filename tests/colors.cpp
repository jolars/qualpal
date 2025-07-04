#include "../src/qualpal/colors.h"
#include "../src/qualpal/color_grid.h"
#include "../src/qualpal/cvd_simulation.h"
#include "../src/qualpal/qualpal.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace qualpal;
using namespace Catch::Matchers;

TEST_CASE("All color conversions are supported", "[colors]")
{

  XYZ xyz(0.4, 0.2, 0.65);
  RGB rgb(0.23, 0.5, 0.95);
  HSL hsl(190, 0.5, 1.0);
  Lab lab(0.3, 0.98, 0.25);
  DIN99d din99d(0.3, 0.98, 0.25);

  rgb = xyz;
  hsl = xyz;
  lab = xyz;
  din99d = xyz;

  REQUIRE_THAT(rgb.r(), WithinAbsMatcher(0.834916, 0.001));
  REQUIRE_THAT(rgb.g(), WithinAbsMatcher(0.125819, 0.001));
  REQUIRE_THAT(rgb.b(), WithinAbsMatcher(0.837110, 0.001));

  REQUIRE_THAT(lab.l(), WithinAbsMatcher(51.8372, 0.001));
  REQUIRE_THAT(lab.a(), WithinAbsMatcher(82.2926, 0.001));
  REQUIRE_THAT(lab.b(), WithinAbsMatcher(-51.4415, 0.001));

  rgb = hsl;
  xyz = hsl;
  lab = hsl;
  din99d = hsl;

  rgb = lab;
  xyz = lab;
  lab = lab;
  din99d = lab;

  REQUIRE_THAT(rgb.r(), WithinAbsMatcher(0.834916, 0.001));
  REQUIRE_THAT(rgb.g(), WithinAbsMatcher(0.125819, 0.001));
  REQUIRE_THAT(rgb.b(), WithinAbsMatcher(0.837110, 0.001));

  REQUIRE_THAT(xyz.x(), WithinAbsMatcher(0.400000, 0.001));
  REQUIRE_THAT(xyz.y(), WithinAbsMatcher(0.200000, 0.001));
  REQUIRE_THAT(xyz.z(), WithinAbsMatcher(0.650000, 0.001));
}

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

  REQUIRE_THAT(hsl.h(), WithinAbsMatcher(217.5, 0.001));
  REQUIRE_THAT(hsl.s(), WithinAbsMatcher(0.87805, 0.001));
  REQUIRE_THAT(hsl.l(), WithinAbsMatcher(0.59, 0.001));

  qualpal::RGB rgb_from_hex("#3b80f2");

  REQUIRE_THAT(rgb_from_hex.r(), WithinAbsMatcher(0.231372, 0.001));
  REQUIRE_THAT(rgb_from_hex.g(), WithinAbsMatcher(0.501960, 0.001));
  REQUIRE_THAT(rgb_from_hex.b(), WithinAbsMatcher(0.949019, 0.001));
}

TEST_CASE("Check that converted HSL colors are within correct ranges",
          "[colors]")
{
  using namespace Catch::Matchers;

  qualpal::RGB rgb(0.8109190, 0.4385978, 0.4837288);
  qualpal::HSL hsl(rgb);

  REQUIRE(hsl.h() >= 0);
  REQUIRE(hsl.h() <= 360);
  REQUIRE(hsl.s() >= 0);
  REQUIRE(hsl.s() <= 1);
  REQUIRE(hsl.l() >= 0);
  REQUIRE(hsl.l() <= 1);
}

TEST_CASE("RGB from HSL conversion", "[colors][rgb][hsl]")
{
  SECTION("Primary colors")
  {
    HSL red_hsl(0.0, 1.0, 0.5);
    RGB red_rgb(red_hsl);
    REQUIRE_THAT(red_rgb.r(), WithinAbs(1.0, 1e-6));
    REQUIRE_THAT(red_rgb.g(), WithinAbs(0.0, 1e-6));
    REQUIRE_THAT(red_rgb.b(), WithinAbs(0.0, 1e-6));

    HSL green_hsl(120.0, 1.0, 0.5);
    RGB green_rgb(green_hsl);
    REQUIRE_THAT(green_rgb.r(), WithinAbs(0.0, 1e-6));
    REQUIRE_THAT(green_rgb.g(), WithinAbs(1.0, 1e-6));
    REQUIRE_THAT(green_rgb.b(), WithinAbs(0.0, 1e-6));
  }

  SECTION("Round-trip conversion")
  {
    RGB original(0.7, 0.3, 0.9);
    HSL hsl(original);
    RGB converted(hsl);

    REQUIRE_THAT(converted.r(), WithinAbs(original.r(), 1e-6));
    REQUIRE_THAT(converted.g(), WithinAbs(original.g(), 1e-6));
    REQUIRE_THAT(converted.b(), WithinAbs(original.b(), 1e-6));
  }
}

TEST_CASE("XYZ constructor", "[colors][xyz]")
{
  XYZ xyz(0.5, 0.3, 0.2);
  REQUIRE_THAT(xyz.x(), WithinAbs(0.5, 1e-10));
  REQUIRE_THAT(xyz.y(), WithinAbs(0.3, 1e-10));
  REQUIRE_THAT(xyz.z(), WithinAbs(0.2, 1e-10));
}

TEST_CASE("Lab constructor", "[colors][lab]")
{
  Lab lab(50.0, 20.0, -10.0);
  REQUIRE_THAT(lab.l(), WithinAbs(50.0, 1e-10));
  REQUIRE_THAT(lab.a(), WithinAbs(20.0, 1e-10));
  REQUIRE_THAT(lab.b(), WithinAbs(-10.0, 1e-10));
}

TEST_CASE("DIN99d constructor", "[colors][din99d]")
{
  DIN99d din(40.0, 15.0, -5.0);
  REQUIRE_THAT(din.l(), WithinAbs(40.0, 1e-10));
  REQUIRE_THAT(din.a(), WithinAbs(15.0, 1e-10));
  REQUIRE_THAT(din.b(), WithinAbs(-5.0, 1e-10));
}

TEST_CASE("RGB constructor from values", "[colors][rgb]")
{
  RGB rgb(1.0, 0.5, 0.0);
  REQUIRE_THAT(rgb.r(), WithinAbs(1.0, 1e-10));
  REQUIRE_THAT(rgb.g(), WithinAbs(0.5, 1e-10));
  REQUIRE_THAT(rgb.b(), WithinAbs(0.0, 1e-10));
}

TEST_CASE("RGB constructor from hex", "[colors][rgb]")
{
  SECTION("6-digit hex")
  {
    RGB rgb("#ff8000");
    REQUIRE_THAT(rgb.r(), WithinAbs(1.0, 1e-3));
    REQUIRE_THAT(rgb.g(), WithinAbs(0.502, 1e-3)); // 128/255
    REQUIRE_THAT(rgb.b(), WithinAbs(0.0, 1e-3));
  }

  SECTION("3-digit hex")
  {
    RGB rgb("#f80");
    REQUIRE_THAT(rgb.r(), WithinAbs(1.0, 1e-3));
    REQUIRE_THAT(rgb.g(), WithinAbs(0.533, 1e-3)); // 0x88/255
    REQUIRE_THAT(rgb.b(), WithinAbs(0.0, 1e-3));
  }

  SECTION("Black and white")
  {
    RGB black("#000000");
    RGB white("#ffffff");

    REQUIRE_THAT(black.r(), WithinAbs(0.0, 1e-10));
    REQUIRE_THAT(black.g(), WithinAbs(0.0, 1e-10));
    REQUIRE_THAT(black.b(), WithinAbs(0.0, 1e-10));

    REQUIRE_THAT(white.r(), WithinAbs(1.0, 1e-3));
    REQUIRE_THAT(white.g(), WithinAbs(1.0, 1e-3));
    REQUIRE_THAT(white.b(), WithinAbs(1.0, 1e-3));
  }
}

TEST_CASE("RGB hex() output", "[colors][rgb]")
{
  SECTION("Red color")
  {
    RGB red(1.0, 0.0, 0.0);
    REQUIRE(red.hex() == "#ff0000");
  }

  SECTION("Mixed color")
  {
    RGB color(0.5, 0.75, 0.25);
    // 0.5*255=127.5->127, 0.75*255=191.25->191, 0.25*255=63.75->63
    REQUIRE(color.hex() == "#7fbf3f");
  }
}

TEST_CASE("HSL constructor", "[colors][hsl]")
{
  HSL hsl(180.0, 0.5, 0.75);
  REQUIRE_THAT(hsl.h(), WithinAbs(180.0, 1e-10));
  REQUIRE_THAT(hsl.s(), WithinAbs(0.5, 1e-10));
  REQUIRE_THAT(hsl.l(), WithinAbs(0.75, 1e-10));
}

TEST_CASE("HSL from RGB conversion", "[colors][hsl][rgb]")
{
  SECTION("Red")
  {
    RGB red(1.0, 0.0, 0.0);
    HSL hsl(red);
    REQUIRE_THAT(hsl.h(), WithinAbs(0.0, 1e-6));
    REQUIRE_THAT(hsl.s(), WithinAbs(1.0, 1e-6));
    REQUIRE_THAT(hsl.l(), WithinAbs(0.5, 1e-6));
  }

  SECTION("Green")
  {
    RGB green(0.0, 1.0, 0.0);
    HSL hsl(green);
    REQUIRE_THAT(hsl.h(), WithinAbs(120.0, 1e-6));
    REQUIRE_THAT(hsl.s(), WithinAbs(1.0, 1e-6));
    REQUIRE_THAT(hsl.l(), WithinAbs(0.5, 1e-6));
  }

  SECTION("Grayscale")
  {
    RGB gray(0.5, 0.5, 0.5);
    HSL hsl(gray);
    REQUIRE_THAT(hsl.h(), WithinAbs(0.0, 1e-6));
    REQUIRE_THAT(hsl.s(), WithinAbs(0.0, 1e-6));
    REQUIRE_THAT(hsl.l(), WithinAbs(0.5, 1e-6));
  }
}
