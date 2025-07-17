#pragma once

#include <map>
#include <qualpal/color_difference.h>
#include <qualpal/colors.h>
#include <qualpal/matrix.h>

namespace qualpal {

struct PaletteAnalysis
{
  Matrix<double> difference_matrix;
  std::vector<double> min_distances;
};

enum class CvdType
{
  Normal,
  Deutan,
  Protan,
  Tritan
};

using PaletteAnalysisMap = std::map<std::string, PaletteAnalysis>;

PaletteAnalysisMap
analyzePalette(const std::vector<colors::RGB>& colors,
               metrics::MetricType metric = metrics::MetricType::DIN99d,
               double severity = 1.0,
               double max_memory = 1.0);

} // namespace qualpal
