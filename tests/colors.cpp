#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal/colors.h>

TEST_CASE("All color conversions are supported", "[colors]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  XYZ xyz(0.4, 0.2, 0.65);
  RGB rgb;
  HSL hsl;
  Lab lab;
  DIN99d din99d;
  LCHab lchab;

  rgb = xyz;
  hsl = xyz;
  lab = xyz;
  din99d = xyz;
  lchab = xyz;

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
  lchab = hsl;

  rgb = lab;
  xyz = lab;
  din99d = lab;
  lchab = lab;

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
  using namespace qualpal::colors;

  RGB rgb(0.23, 0.5, 0.95);
  XYZ xyz(rgb);
  HSL hsl(rgb);
  DIN99d din99d(xyz);
  Lab lab(xyz);

  REQUIRE_THAT(xyz.x(), WithinAbsMatcher(0.254958, 0.001));
  REQUIRE_THAT(xyz.y(), WithinAbsMatcher(0.226503, 0.001));
  REQUIRE_THAT(xyz.z(), WithinAbsMatcher(0.872124, 0.001));

  REQUIRE_THAT(lab.l(), WithinAbsMatcher(54.7103, 0.001));
  REQUIRE_THAT(lab.a(), WithinAbsMatcher(17.6776, 0.001));
  REQUIRE_THAT(lab.b(), WithinAbsMatcher(-63.8246, 0.001));

  REQUIRE_THAT(hsl.h(), WithinAbsMatcher(217.5, 0.001));
  REQUIRE_THAT(hsl.s(), WithinAbsMatcher(0.87805, 0.001));
  REQUIRE_THAT(hsl.l(), WithinAbsMatcher(0.59, 0.001));

  RGB rgb_from_hex("#3b80f2");

  REQUIRE_THAT(rgb_from_hex.r(), WithinAbsMatcher(0.231372, 0.001));
  REQUIRE_THAT(rgb_from_hex.g(), WithinAbsMatcher(0.501960, 0.001));
  REQUIRE_THAT(rgb_from_hex.b(), WithinAbsMatcher(0.949019, 0.001));
}

TEST_CASE("Check that converted HSL colors are within correct ranges",
          "[colors]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  RGB rgb(0.8109190, 0.4385978, 0.4837288);
  HSL hsl(rgb);

  REQUIRE(hsl.h() >= 0);
  REQUIRE(hsl.h() <= 360);
  REQUIRE(hsl.s() >= 0);
  REQUIRE(hsl.s() <= 1);
  REQUIRE(hsl.l() >= 0);
  REQUIRE(hsl.l() <= 1);
}

TEST_CASE("RGB from HSL conversion", "[colors][rgb][hsl]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

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
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  XYZ xyz(0.5, 0.3, 0.2);
  REQUIRE_THAT(xyz.x(), WithinAbs(0.5, 1e-10));
  REQUIRE_THAT(xyz.y(), WithinAbs(0.3, 1e-10));
  REQUIRE_THAT(xyz.z(), WithinAbs(0.2, 1e-10));
}

TEST_CASE("Lab constructor", "[colors][lab]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  Lab lab(50.0, 20.0, -10.0);
  REQUIRE_THAT(lab.l(), WithinAbs(50.0, 1e-10));
  REQUIRE_THAT(lab.a(), WithinAbs(20.0, 1e-10));
  REQUIRE_THAT(lab.b(), WithinAbs(-10.0, 1e-10));
}

TEST_CASE("DIN99d constructor", "[colors][din99d]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  DIN99d din(40.0, 15.0, -5.0);
  REQUIRE_THAT(din.l(), WithinAbs(40.0, 1e-10));
  REQUIRE_THAT(din.a(), WithinAbs(15.0, 1e-10));
  REQUIRE_THAT(din.b(), WithinAbs(-5.0, 1e-10));
}

TEST_CASE("LCHab constructor", "[colors][lchab]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  LCHab lch(60.0, 30.0, 120.0);
  REQUIRE_THAT(lch.l(), WithinAbs(60.0, 1e-10));
  REQUIRE_THAT(lch.c(), WithinAbs(30.0, 1e-10));
  REQUIRE_THAT(lch.h(), WithinAbs(120.0, 1e-10));
}

TEST_CASE("RGB constructor from values", "[colors][rgb]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  RGB rgb(1.0, 0.5, 0.0);
  REQUIRE_THAT(rgb.r(), WithinAbs(1.0, 1e-10));
  REQUIRE_THAT(rgb.g(), WithinAbs(0.5, 1e-10));
  REQUIRE_THAT(rgb.b(), WithinAbs(0.0, 1e-10));
}

TEST_CASE("RGB constructor from hex", "[colors][rgb]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

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
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  SECTION("Red color")
  {
    RGB red(1.0, 0.0, 0.0);
    REQUIRE(red.hex() == "#ff0000");
  }

  SECTION("Mixed color")
  {
    RGB color(0.5, 0.75, 0.25);
    // 0.5*255=127.5->127, 0.75*255=191.25->191, 0.25*255=63.75->63
    REQUIRE(color.hex() == "#80bf40");
  }

  SECTION("Black")
  {
    RGB black(0.0, 0.0, 0.0);
    REQUIRE(black.hex() == "#000000");
  }

  SECTION("White")
  {
    RGB white(1.0, 1.0, 1.0);
    REQUIRE(white.hex() == "#ffffff");
  }

  SECTION("Rounding up to 255")
  {
    RGB almost_white(0.999999, 0.999999, 0.999999);
    REQUIRE(almost_white.hex() == "#ffffff");
  }

  SECTION("Rounding down to 0")
  {
    RGB almost_black(0.000001, 0.000001, 0.000001);
    REQUIRE(almost_black.hex() == "#000000");
  }

  SECTION("Middle gray")
  {
    RGB gray(0.5, 0.5, 0.5);
    REQUIRE(gray.hex() == "#808080");
  }
}

TEST_CASE("HSL constructor", "[colors][hsl]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  HSL hsl(180.0, 0.5, 0.75);
  REQUIRE_THAT(hsl.h(), WithinAbs(180.0, 1e-10));
  REQUIRE_THAT(hsl.s(), WithinAbs(0.5, 1e-10));
  REQUIRE_THAT(hsl.l(), WithinAbs(0.75, 1e-10));
}

TEST_CASE("HSL from RGB conversion", "[colors][hsl][rgb]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

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

TEST_CASE("RGB conversion", "[colors][rgb]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  RGB rgb(0.34, 0.98, 0.01);

  double eps = 1e-4;

  SECTION("To Lab")
  {
    Lab lab(rgb);
    REQUIRE_THAT(lab.l(), WithinAbs(87.1549, eps));
    REQUIRE_THAT(lab.a(), WithinAbs(-76.0668, eps));
    REQUIRE_THAT(lab.b(), WithinAbs(82.9309, eps));
  }

  SECTION("To XYZ")
  {
    XYZ xyz(rgb);
    REQUIRE_THAT(xyz.x(), WithinAbs(0.380693, eps));
    REQUIRE_THAT(xyz.y(), WithinAbs(0.703226, eps));
    REQUIRE_THAT(xyz.z(), WithinAbs(0.116406, eps));
  }
}

TEST_CASE("XYZ conversion", "[colors][xyz]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  XYZ xyz(0.4, 0.5, 0.99);

  double eps = 1e-4;

  SECTION("To Lab")
  {
    Lab lab(xyz);
    REQUIRE_THAT(lab.l(), WithinAbs(76.0693, eps));
    REQUIRE_THAT(lab.a(), WithinAbs(-22.1559, eps));
    REQUIRE_THAT(lab.b(), WithinAbs(-35.0158, eps));
  }

  SECTION("To RGB")
  {
    RGB rgb(xyz);
    REQUIRE_THAT(rgb.r(), WithinAbs(0.20306, eps));
    REQUIRE_THAT(rgb.g(), WithinAbs(0.792647, eps));
    REQUIRE_THAT(rgb.b(), WithinAbs(0.985306, eps));
  }

  // From Cui et al 2002
  std::array<double, 3> ref_white = { 0.9481, 1, 1.0733 };
  XYZ xyz_sample1(0.0496, 0.0372, 0.1959);
  XYZ xyz_sample2(0.046651, 0.0381, 0.177848);

  SECTION("To DIN99d")
  {
    DIN99d din99d1(xyz_sample1, ref_white);

    REQUIRE_THAT(din99d1.l(), WithinAbs(25.572, eps));
    REQUIRE_THAT(din99d1.a(), WithinAbs(-0.9627, eps));
    REQUIRE_THAT(din99d1.b(), WithinAbs(-30.8731, eps));

    DIN99d din99d2(xyz_sample2, ref_white);

    REQUIRE_THAT(din99d2.l(), WithinAbs(25.9071, eps));
    REQUIRE_THAT(din99d2.a(), WithinAbs(-3.5163, eps));
    REQUIRE_THAT(din99d2.b(), WithinAbs(-29.1561, eps));
  }
}

TEST_CASE("HSL conversion", "[colors][hsl]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  HSL hsl(309, 0.2, 0.86);

  double eps = 1e-4;

  SECTION("To RGB")
  {
    RGB rgb(hsl);
    REQUIRE_THAT(rgb.r(), WithinAbs(0.888, eps));
    REQUIRE_THAT(rgb.g(), WithinAbs(0.832, eps));
    REQUIRE_THAT(rgb.b(), WithinAbs(0.8796, eps));
  }
}

TEST_CASE("Lab conversion", "[colors][lab]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  Lab lab1(23, 12, 17);
  Lab lab2(50, -60, 10);
  Lab lab3(20, -23, 12);
  Lab lab4(49, -8, 9);

  double eps = 1e-4;

  SECTION("To XYZ")
  {
    XYZ xyz1(lab1);
    REQUIRE_THAT(xyz1.x(), WithinAbs(0.044422, eps));
    REQUIRE_THAT(xyz1.y(), WithinAbs(0.038003, eps));
    REQUIRE_THAT(xyz1.z(), WithinAbs(0.017261, eps));

    XYZ xyz2(lab2);
    REQUIRE_THAT(xyz2.x(), WithinAbs(0.086016, eps));
    REQUIRE_THAT(xyz2.y(), WithinAbs(0.184187, eps));
    REQUIRE_THAT(xyz2.z(), WithinAbs(0.152186, eps));
  }

  SECTION("To RGB")
  {
    RGB rgb(lab3);
    REQUIRE_THAT(rgb.r(), WithinAbs(0.031398, eps));
    REQUIRE_THAT(rgb.g(), WithinAbs(0.220247, eps));
    REQUIRE_THAT(rgb.b(), WithinAbs(0.117406, eps));
  }

  SECTION("To HSL")
  {
    LCHab lch(lab4);
    REQUIRE_THAT(lch.l(), WithinAbs(49, eps));
    REQUIRE_THAT(lch.c(), WithinAbs(12.0416, eps));
    REQUIRE_THAT(lch.h(), WithinAbs(131.6335, eps));
  }
}

TEST_CASE("LCHab conversion", "[colors][lchab]")
{
  using namespace Catch::Matchers;
  using namespace qualpal::colors;

  LCHab lch1(23, 12, 17);

  double eps = 1e-4;

  SECTION("To XYZ")
  {
    XYZ xyz1(lch1);
    REQUIRE_THAT(xyz1.x(), WithinAbs(0.044035, eps));
    REQUIRE_THAT(xyz1.y(), WithinAbs(0.038003, eps));
    REQUIRE_THAT(xyz1.z(), WithinAbs(0.035234, eps));
  }

  SECTION("To RGB")
  {
    RGB rgb(lch1);
    REQUIRE_THAT(rgb.r(), WithinAbs(0.286457, eps));
    REQUIRE_THAT(rgb.g(), WithinAbs(0.190010, eps));
    REQUIRE_THAT(rgb.b(), WithinAbs(0.196245, eps));
  }

  SECTION("To Lab")
  {
    Lab lab(lch1);
    REQUIRE_THAT(lab.l(), WithinAbs(23, eps));
    REQUIRE_THAT(lab.a(), WithinAbs(11.4757, eps));
    REQUIRE_THAT(lab.b(), WithinAbs(3.5085, eps));
  }
}

TEST_CASE("Braced initializer lists for color construction", "[colors]")
{
  std::vector<qualpal::colors::RGB> base{ "#ff0000", "#a9ef93", "#ffe302" };
  std::vector<qualpal::colors::RGB> colors{ std::string("#ff0000"),
                                            std::string("#a9ef93"),
                                            std::string("#ffe302") };

  REQUIRE(base.size() == 3);
  REQUIRE(colors.size() == 3);
}
