#include <qualpal/color_difference.h>
#include <qualpal/matrix.h>

namespace qualpal {

Matrix<double>
colorDifferenceMatrix(const std::vector<colors::Lab>& colors,
                      const metrics::MetricType& metric_type,
                      const std::size_t max_memory)
{
  switch (metric_type) {
    case metrics::MetricType::DIN99d: {
      std::vector<colors::DIN99d> din99d_colors;
      din99d_colors.reserve(colors.size());
      for (const auto& col : colors) {
        din99d_colors.emplace_back(col);
      }
      return colorDifferenceMatrix(colors, metrics::DIN99d{}, max_memory);
    }
    case metrics::MetricType::CIEDE2000:
      return colorDifferenceMatrix(colors, metrics::CIEDE2000{}, max_memory);
    default:
      throw std::invalid_argument("Unknown metric type");
  }
}
} // namespace qualpal
