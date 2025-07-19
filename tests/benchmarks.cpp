#include "../src/qualpal/color_grid.h"
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <cmath>
#include <qualpal/color_difference.h>

TEST_CASE("Parallelization", "[!benchmark]")
{
  std::array<double, 2> h_lim = { -360.0, 360.0 };
  std::array<double, 2> s_lim = { 0.0, 1.0 };
  std::array<double, 2> l_lim = { 0.0, 1.0 };

  int n_points = 101;

  auto hsl_colors =
    qualpal::colorGrid<qualpal::colors::HSL>(h_lim, s_lim, l_lim, n_points);

  std::vector<qualpal::colors::RGB> rgb_colors;

  for (auto color : hsl_colors) {
    rgb_colors.emplace_back(color);
  }

  BENCHMARK("Parallel color difference matrix")
  {
    auto result = qualpal::colorDifferenceMatrix(rgb_colors);
  };

  BENCHMARK("Sequential color difference matrix")
  {
    qualpal::Threads::set(1);
    auto result = qualpal::colorDifferenceMatrix(rgb_colors);
  };
}

TEST_CASE("Color Difference Metrics", "[!benchmark]")
{
  std::array<double, 2> h_lim = { -360.0, 360.0 };
  std::array<double, 2> s_lim = { 0.0, 1.0 };
  std::array<double, 2> l_lim = { 0.0, 1.0 };

  int n_points = 1000;

  auto hsl_colors =
    qualpal::colorGrid<qualpal::colors::HSL>(h_lim, s_lim, l_lim, n_points);

  std::vector<qualpal::colors::Lab> lab_colors;
  std::vector<qualpal::colors::DIN99d> din99d_colors;

  for (auto color : hsl_colors) {
    lab_colors.emplace_back(color);
    din99d_colors.emplace_back(color);
  }

  BENCHMARK("DIN99d")
  {
    qualpal::colorDifferenceMatrix(din99d_colors, qualpal::metrics::DIN99d{});
  };

  BENCHMARK("CIEDE2000")
  {
    qualpal::colorDifferenceMatrix(lab_colors, qualpal::metrics::CIEDE2000{});
  };

  BENCHMARK("CIE76")
  {
    qualpal::colorDifferenceMatrix(lab_colors, qualpal::metrics::CIE76{});
  };
}
