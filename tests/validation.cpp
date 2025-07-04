#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal/validation.h>

using namespace qualpal;

TEST_CASE("isValidHexColor validates correctly", "[validation]")
{
  SECTION("Valid hex colors")
  {
    REQUIRE(isValidHexColor("#ff0000"));
    REQUIRE(isValidHexColor("#00FF00"));
    REQUIRE(isValidHexColor("#0000ff"));
    REQUIRE(isValidHexColor("#123456"));
    REQUIRE(isValidHexColor("#abcdef"));
    REQUIRE(isValidHexColor("#ABCDEF"));
    REQUIRE(isValidHexColor("#f00"));
    REQUIRE(isValidHexColor("#0F0"));
    REQUIRE(isValidHexColor("#00f"));
    REQUIRE(isValidHexColor("#abc"));
  }

  SECTION("Invalid hex colors")
  {
    REQUIRE_FALSE(isValidHexColor("ff0000"));    // Missing #
    REQUIRE_FALSE(isValidHexColor("#gg0000"));   // Invalid character
    REQUIRE_FALSE(isValidHexColor("#ff00"));     // Wrong length
    REQUIRE_FALSE(isValidHexColor("#ff000000")); // Too long
    REQUIRE_FALSE(isValidHexColor("#"));         // Just #
    REQUIRE_FALSE(isValidHexColor(""));          // Empty
    REQUIRE_FALSE(isValidHexColor("#ff 000"));   // Space
    REQUIRE_FALSE(isValidHexColor("#ff-000"));   // Hyphen
  }
}

TEST_CASE("validateHslRanges validates correctly", "[validation]")
{
  SECTION("Valid ranges")
  {
    REQUIRE_NOTHROW(validateHslRanges({ 0, 360 }, { 0, 1 }, { 0, 1 }));
    REQUIRE_NOTHROW(
      validateHslRanges({ -180, 180 }, { 0.2, 0.8 }, { 0.1, 0.9 }));
    REQUIRE_NOTHROW(validateHslRanges({ 100, 200 }, { 0, 0.5 }, { 0.3, 0.7 }));
  }

  SECTION("Invalid hue ranges")
  {
    REQUIRE_THROWS_AS(validateHslRanges({ 360, 0 }, { 0, 1 }, { 0, 1 }),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(validateHslRanges({ 100, 100 }, { 0, 1 }, { 0, 1 }),
                      std::invalid_argument);
  }

  SECTION("Invalid saturation ranges")
  {
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 360 }, { 1, 0 }, { 0, 1 }),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 360 }, { -0.1, 1 }, { 0, 1 }),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 360 }, { 0, 1.1 }, { 0, 1 }),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 360 }, { 0.5, 0.5 }, { 0, 1 }),
                      std::invalid_argument);
  }

  SECTION("Invalid lightness ranges")
  {
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 360 }, { 0, 1 }, { 1, 0 }),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 360 }, { 0, 1 }, { -0.1, 1 }),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 360 }, { 0, 1 }, { 0, 1.1 }),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 360 }, { 0, 1 }, { 0.7, 0.7 }),
                      std::invalid_argument);
  }

  SECTION("Edge case: exactly equal bounds")
  {
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 0 }, { 0, 1 }, { 0, 1 }),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 360 }, { 0, 0 }, { 0, 1 }),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(validateHslRanges({ 0, 360 }, { 0, 1 }, { 0, 0 }),
                      std::invalid_argument);
  }

  SECTION("Edge case: boundary values")
  {
    REQUIRE_NOTHROW(validateHslRanges({ 0, 0.1 }, { 0, 0.1 }, { 0, 0.1 }));
    REQUIRE_NOTHROW(validateHslRanges({ 359.9, 360 }, { 0.9, 1 }, { 0.9, 1 }));
  }
}
