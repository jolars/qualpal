#include "../src/qualpal/color_grid.h"
#include "../src/qualpal/cvd_simulation.h"
#include "../src/qualpal/qualpal.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Matrix-Matrix multiplication", "[FixedMatrix]")
{
  qualpal::FixedMatrix<int, 2, 3> A = { { 1, 2, 3 }, { 4, 5, 6 } };
  qualpal::FixedMatrix<int, 3, 2> B = { { 7, 8 }, { 9, 10 }, { 11, 12 } };

  qualpal::FixedMatrix<int, 2, 2> result = A * B;

  REQUIRE(result(0, 0) == 58);
  REQUIRE(result(0, 1) == 64);
  REQUIRE(result(1, 0) == 139);
  REQUIRE(result(1, 1) == 154);
}

TEST_CASE("Matrix-MatrixTranspose multiplication", "[FixedMatrix]")
{
  qualpal::FixedMatrix<int, 2, 3> A = { { 1, 2, 3 }, { 4, 5, 6 } };
  qualpal::FixedMatrix<int, 2, 3> B = { { 7, 8, 9 }, { 10, 11, 12 } };

  qualpal::FixedMatrix<int, 2, 2> result = A * B.t();

  REQUIRE(result(0, 0) == 50);
  REQUIRE(result(0, 1) == 68);
  REQUIRE(result(1, 0) == 122);
  REQUIRE(result(1, 1) == 167);
}
