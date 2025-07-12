#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <qualpal/matrix.h>

using namespace qualpal;
using namespace Catch::Matchers;

TEST_CASE("Dynamic Matrix construction", "[matrix][dynamic]")
{
  SECTION("Basic construction")
  {
    Matrix<double> mat(3, 4);
    REQUIRE(mat.nrow() == 3);
    REQUIRE(mat.ncol() == 4);
  }

  SECTION("Construction with data")
  {
    std::vector<double> data = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
    Matrix<double> mat(2, 3, data);

    REQUIRE(mat.nrow() == 2);
    REQUIRE(mat.ncol() == 3);

    // Check column-major ordering
    REQUIRE_THAT(mat(0, 0), WithinAbs(1.0, 1e-10));
    REQUIRE_THAT(mat(1, 0), WithinAbs(2.0, 1e-10));
    REQUIRE_THAT(mat(0, 1), WithinAbs(3.0, 1e-10));
    REQUIRE_THAT(mat(1, 1), WithinAbs(4.0, 1e-10));
    REQUIRE_THAT(mat(0, 2), WithinAbs(5.0, 1e-10));
    REQUIRE_THAT(mat(1, 2), WithinAbs(6.0, 1e-10));
  }
}

TEST_CASE("Dynamic Matrix element access", "[matrix][dynamic]")
{
  Matrix<double> mat(2, 3);

  // Test mutable access
  mat(0, 0) = 1.5;
  mat(1, 2) = 3.7;

  REQUIRE_THAT(mat(0, 0), WithinAbs(1.5, 1e-10));
  REQUIRE_THAT(mat(1, 2), WithinAbs(3.7, 1e-10));

  // Test const access
  const Matrix<double>& const_mat = mat;
  REQUIRE_THAT(const_mat(0, 0), WithinAbs(1.5, 1e-10));
  REQUIRE_THAT(const_mat(1, 2), WithinAbs(3.7, 1e-10));
}

TEST_CASE("Dynamic Matrix transpose", "[matrix][dynamic]")
{
  std::vector<double> data = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
  Matrix<double> mat(2, 3, data);

  Matrix<double> transposed = mat.transpose();

  REQUIRE(transposed.nrow() == 3);
  REQUIRE(transposed.ncol() == 2);

  REQUIRE_THAT(transposed(0, 0), WithinAbs(1.0, 1e-10));
  REQUIRE_THAT(transposed(0, 1), WithinAbs(2.0, 1e-10));
  REQUIRE_THAT(transposed(1, 0), WithinAbs(3.0, 1e-10));
  REQUIRE_THAT(transposed(1, 1), WithinAbs(4.0, 1e-10));
  REQUIRE_THAT(transposed(2, 0), WithinAbs(5.0, 1e-10));
  REQUIRE_THAT(transposed(2, 1), WithinAbs(6.0, 1e-10));
}

TEST_CASE("FixedMatrix construction", "[matrix][fixed]")
{
  SECTION("Default construction")
  {
    FixedMatrix<double, 2, 3> mat;
    // Elements should be zero-initialized
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 3; ++j) {
        REQUIRE_THAT(mat(i, j), WithinAbs(0.0, 1e-10));
      }
    }
  }

  SECTION("Initializer list construction")
  {
    FixedMatrix<double, 2, 3> mat = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };

    REQUIRE_THAT(mat(0, 0), WithinAbs(1.0, 1e-10));
    REQUIRE_THAT(mat(0, 1), WithinAbs(2.0, 1e-10));
    REQUIRE_THAT(mat(0, 2), WithinAbs(3.0, 1e-10));
    REQUIRE_THAT(mat(1, 0), WithinAbs(4.0, 1e-10));
    REQUIRE_THAT(mat(1, 1), WithinAbs(5.0, 1e-10));
    REQUIRE_THAT(mat(1, 2), WithinAbs(6.0, 1e-10));
  }

  SECTION("Partial initializer list")
  {
    FixedMatrix<double, 3, 3> mat = { { 1.0, 2.0 }, { 3.0 } };

    REQUIRE_THAT(mat(0, 0), WithinAbs(1.0, 1e-10));
    REQUIRE_THAT(mat(0, 1), WithinAbs(2.0, 1e-10));
    REQUIRE_THAT(mat(0, 2), WithinAbs(0.0, 1e-10)); // Should be zero
    REQUIRE_THAT(mat(1, 0), WithinAbs(3.0, 1e-10));
    REQUIRE_THAT(mat(1, 1), WithinAbs(0.0, 1e-10)); // Should be zero
  }
}

TEST_CASE("FixedMatrix element access", "[matrix][fixed]")
{
  FixedMatrix<double, 2, 3> mat = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };

  // Test mutable access
  mat(1, 1) = 10.5;
  REQUIRE_THAT(mat(1, 1), WithinAbs(10.5, 1e-10));

  // Test const access
  const FixedMatrix<double, 2, 3>& const_mat = mat;
  REQUIRE_THAT(const_mat(0, 0), WithinAbs(1.0, 1e-10));
  REQUIRE_THAT(const_mat(1, 1), WithinAbs(10.5, 1e-10));
}

TEST_CASE("FixedMatrix transpose", "[matrix][fixed]")
{
  FixedMatrix<double, 2, 3> mat = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };

  auto transposed = mat.t();

  REQUIRE_THAT(transposed(0, 0), WithinAbs(1.0, 1e-10));
  REQUIRE_THAT(transposed(0, 1), WithinAbs(4.0, 1e-10));
  REQUIRE_THAT(transposed(1, 0), WithinAbs(2.0, 1e-10));
  REQUIRE_THAT(transposed(1, 1), WithinAbs(5.0, 1e-10));
  REQUIRE_THAT(transposed(2, 0), WithinAbs(3.0, 1e-10));
  REQUIRE_THAT(transposed(2, 1), WithinAbs(6.0, 1e-10));
}

TEST_CASE("FixedMatrix scalar multiplication", "[matrix][fixed]")
{
  FixedMatrix<double, 2, 2> mat = { { 1.0, 2.0 }, { 3.0, 4.0 } };

  auto result = mat * 2.5;

  REQUIRE_THAT(result(0, 0), WithinAbs(2.5, 1e-10));
  REQUIRE_THAT(result(0, 1), WithinAbs(5.0, 1e-10));
  REQUIRE_THAT(result(1, 0), WithinAbs(7.5, 1e-10));
  REQUIRE_THAT(result(1, 1), WithinAbs(10.0, 1e-10));
}

TEST_CASE("FixedMatrix vector multiplication", "[matrix][fixed]")
{
  FixedMatrix<double, 2, 3> mat = { { 1.0, 2.0, 3.0 }, { 4.0, 5.0, 6.0 } };
  std::array<double, 3> vec = { 1.0, 2.0, 3.0 };

  auto result = mat * vec;

  // First row: 1*1 + 2*2 + 3*3 = 14
  // Second row: 4*1 + 5*2 + 6*3 = 32
  REQUIRE_THAT(result[0], WithinAbs(14.0, 1e-10));
  REQUIRE_THAT(result[1], WithinAbs(32.0, 1e-10));
}

TEST_CASE("FixedMatrix matrix multiplication", "[matrix][fixed]")
{
  FixedMatrix<int, 2, 3> A = { { 1, 2, 3 }, { 4, 5, 6 } };
  FixedMatrix<int, 3, 2> B = { { 7, 8 }, { 9, 10 }, { 11, 12 } };

  auto result = A * B;

  REQUIRE(result(0, 0) == 58);  // 1*7 + 2*9 + 3*11
  REQUIRE(result(0, 1) == 64);  // 1*8 + 2*10 + 3*12
  REQUIRE(result(1, 0) == 139); // 4*7 + 5*9 + 6*11
  REQUIRE(result(1, 1) == 154); // 4*8 + 5*10 + 6*12
}

TEST_CASE("FixedMatrix dynamic matrix multiplication",
          "[matrix][fixed][dynamic]")
{
  FixedMatrix<double, 2, 3> fixed_mat = { { 1.0, 2.0, 3.0 },
                                          { 4.0, 5.0, 6.0 } };

  std::vector<double> data = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 }; // 3x2 matrix
  Matrix<double> dyn_mat(3, 2, data);

  auto result = fixed_mat * dyn_mat;

  REQUIRE(result.nrow() == 2);
  REQUIRE(result.ncol() == 2);

  REQUIRE_THAT(result(0, 0), WithinAbs(14.0, 1e-10)); // 1*1 + 2*2 + 3*3 = 14
  REQUIRE_THAT(result(0, 1), WithinAbs(32.0, 1e-10)); // 1*4 + 2*5 + 3*6 = 32
  REQUIRE_THAT(result(1, 0), WithinAbs(32.0, 1e-10)); // 4*1 + 5*2 + 6*3 = 32
  REQUIRE_THAT(result(1, 1), WithinAbs(77.0, 1e-10)); // 4*4 + 5*5 + 6*6 = 77
}

TEST_CASE("FixedMatrix addition", "[matrix][fixed]")
{
  FixedMatrix<double, 2, 2> A = { { 1.0, 2.0 }, { 3.0, 4.0 } };
  FixedMatrix<double, 2, 2> B = { { 5.0, 6.0 }, { 7.0, 8.0 } };

  auto result = A + B;

  REQUIRE_THAT(result(0, 0), WithinAbs(6.0, 1e-10));
  REQUIRE_THAT(result(0, 1), WithinAbs(8.0, 1e-10));
  REQUIRE_THAT(result(1, 0), WithinAbs(10.0, 1e-10));
  REQUIRE_THAT(result(1, 1), WithinAbs(12.0, 1e-10));
}

TEST_CASE("FixedMatrix subtraction", "[matrix][fixed]")
{
  FixedMatrix<double, 2, 2> A = { { 5.0, 6.0 }, { 7.0, 8.0 } };
  FixedMatrix<double, 2, 2> B = { { 1.0, 2.0 }, { 3.0, 4.0 } };

  auto result = A - B;

  REQUIRE_THAT(result(0, 0), WithinAbs(4.0, 1e-10));
  REQUIRE_THAT(result(0, 1), WithinAbs(4.0, 1e-10));
  REQUIRE_THAT(result(1, 0), WithinAbs(4.0, 1e-10));
  REQUIRE_THAT(result(1, 1), WithinAbs(4.0, 1e-10));
}

TEST_CASE("FixedMatrix fill and zeros", "[matrix][fixed]")
{
  FixedMatrix<double, 3, 3> mat = { { 1.0, 2.0, 3.0 },
                                    { 4.0, 5.0, 6.0 },
                                    { 7.0, 8.0, 9.0 } };

  SECTION("Fill with value")
  {
    mat.fill(2.5);
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        REQUIRE_THAT(mat(i, j), WithinAbs(2.5, 1e-10));
      }
    }
  }

  SECTION("Fill with zeros")
  {
    mat.zeros();
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        REQUIRE_THAT(mat(i, j), WithinAbs(0.0, 1e-10));
      }
    }
  }
}

TEST_CASE("Matrix constexpr operations", "[matrix][fixed][constexpr]")
{
  // Test that constexpr operations work at compile time
  constexpr FixedMatrix<double, 2, 2> A = { { 1.0, 2.0 }, { 3.0, 4.0 } };
  constexpr FixedMatrix<double, 2, 2> B = { { 2.0, 0.0 }, { 1.0, 2.0 } };

  constexpr auto sum = A + B;
  constexpr auto diff = A - B;
  constexpr auto scaled = A * 2.0;
  constexpr auto transposed = A.t();

  REQUIRE_THAT(sum(0, 0), WithinAbs(3.0, 1e-10));
  REQUIRE_THAT(diff(1, 1), WithinAbs(2.0, 1e-10));
  REQUIRE_THAT(scaled(1, 0), WithinAbs(6.0, 1e-10));
  REQUIRE_THAT(transposed(0, 1), WithinAbs(3.0, 1e-10));
}

TEST_CASE("Matrix with different types", "[matrix][fixed][types]")
{
  SECTION("Integer matrices")
  {
    FixedMatrix<int, 2, 2> mat = { { 1, 2 }, { 3, 4 } };
    auto result = mat * 3;

    REQUIRE(result(0, 0) == 3);
    REQUIRE(result(1, 1) == 12);
  }

  SECTION("Float matrices")
  {
    FixedMatrix<float, 2, 2> mat = { { 1.5f, 2.5f }, { 3.5f, 4.5f } };
    auto result = mat + mat;

    REQUIRE_THAT(result(0, 0), WithinAbs(3.0f, 1e-6f));
    REQUIRE_THAT(result(1, 1), WithinAbs(9.0f, 1e-6f));
  }
}

TEST_CASE("Matrix edge cases", "[matrix][edge-cases]")
{
  SECTION("1x1 matrix")
  {
    FixedMatrix<double, 1, 1> mat = { { 5.0 } };
    auto result = mat * 2.0;

    REQUIRE_THAT(result(0, 0), WithinAbs(10.0, 1e-10));
  }

  SECTION("Row vector")
  {
    FixedMatrix<double, 1, 3> row = { { 1.0, 2.0, 3.0 } };
    FixedMatrix<double, 3, 1> col = { { 4.0 }, { 5.0 }, { 6.0 } };

    auto result = row * col;
    REQUIRE_THAT(result(0, 0), WithinAbs(32.0, 1e-10)); // 1*4 + 2*5 + 3*6
  }

  SECTION("Column vector")
  {
    FixedMatrix<double, 3, 1> col = { { 1.0 }, { 2.0 }, { 3.0 } };
    FixedMatrix<double, 1, 3> row = { { 4.0, 5.0, 6.0 } };

    auto result = col * row;
    REQUIRE_THAT(result(0, 0), WithinAbs(4.0, 1e-10));
    REQUIRE_THAT(result(1, 1), WithinAbs(10.0, 1e-10));
    REQUIRE_THAT(result(2, 2), WithinAbs(18.0, 1e-10));
  }
}
