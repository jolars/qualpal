#include "cvd.h"
#include <algorithm>
#include <qualpal/analyze.h>

namespace qualpal {

PaletteAnalysisMap
analyzePalette(const std::vector<colors::RGB>& colors,
               metrics::MetricType metric,
               double severity,
               double max_memory)
{
  PaletteAnalysisMap result;
  std::vector<std::string> cvd_types = {
    "normal", "deutan", "protan", "tritan"
  };

  for (const auto& cvd_type : cvd_types) {
    std::vector<colors::RGB> simulated_colors = colors;

    if (cvd_type != "normal" && severity > 0.0) {
      for (auto& rgb : simulated_colors) {
        rgb = simulateCvd(rgb, cvd_type, severity);
      }
    }

    std::vector<colors::Lab> xyz_colors;
    xyz_colors.reserve(simulated_colors.size());

    for (const auto& rgb : simulated_colors) {
      xyz_colors.emplace_back(rgb);
    }

    Matrix<double> diff_matrix =
      colorDifferenceMatrix(xyz_colors, metric, max_memory);
    std::vector<double> min_distances;
    min_distances.reserve(diff_matrix.nrow());

    for (int i = 0; i < diff_matrix.nrow(); ++i) {
      double min_dist = std::numeric_limits<double>::max();

      for (int j = 0; j < diff_matrix.ncol(); ++j) {
        if (i != j) {
          min_dist = std::min(min_dist, diff_matrix(i, j));
        }
      }

      min_distances.push_back(min_dist);
    }

    result[cvd_type] = PaletteAnalysis{ diff_matrix, min_distances };
  }
  return result;
}

} // namespace qualpal
