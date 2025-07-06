#pragma once

#include <cmath>
#include <qualpal_bits/matrix.h>
#include <qualpal_bits/metrics.h>
#include <vector>

namespace qualpal {

/**
 * @brief Generate a color difference matrix
 * @tparam ColorType Any color class (colors::RGB, colors::HSL, colors::XYZ,
 * colors::Lab, colors::DIN99d)
 * @tparam Metric Color difference metric (defaults to metrics::DIN99d)
 * @param colors Vector of colors to compare
 * @param metric Color difference metric to use
 * @return Symmetric matrix of pairwise color differences
 */
template<typename ColorType, typename Metric = metrics::DIN99d>
Matrix<double>
colorDifferenceMatrix(const std::vector<ColorType>& colors,
                      const Metric& metric = Metric{})
{
  const int N = colors.size();
  Matrix<double> result(N, N);

  for (int i = 0; i < N; ++i) {
    result(i, i) = 0.0;
    for (int j = i + 1; j < N; ++j) {
      double d = metric(colors[i], colors[j]);
      result(i, j) = d;
      result(j, i) = d;
    }
  }
  return result;
}

} // namespace qualpal
