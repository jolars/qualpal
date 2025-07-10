#pragma once

#include <cmath>
#include <qualpal/matrix.h>
#include <qualpal/metrics.h>
#include <qualpal/threads.h>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

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

#ifdef _OPENMP
#pragma omp parallel for num_threads(Threads::get())
#endif
  for (int i = 0; i < N; ++i) {
    result(i, i) = 0.0;
#ifdef _OPENMP
#pragma omp simd
#endif
    for (int j = i + 1; j < N; ++j) {
      double d = metric(colors[i], colors[j]);
      result(i, j) = d;
      result(j, i) = d;
    }
  }
  return result;
}

} // namespace qualpal
