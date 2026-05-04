// Benchmark for tuning candidate count and distribution.
//
// Output is CSV on stdout. For each (sampler, n_points, palette_size) it
// reports min and median-of-min CIEDE2000 inside the resulting palette, plus
// total wallclock for generate(). Quality "saturates" when increasing n_points
// no longer increases min CIEDE2000.
//
// Build: cmake -B build -S . -DBUILD_TUNING=ON && cmake --build build
// Run:   ./build/tools/tune_candidates > tune.csv

#include "../src/qualpal/color_grid.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <qualpal/color_difference.h>
#include <qualpal/colors.h>
#include <qualpal/metrics.h>
#include <qualpal/qualpal.h>
#include <random>
#include <string>
#include <vector>

namespace {

// HSL configurations under test.
struct HslConfig
{
  const char* name;
  std::array<double, 2> h_lim;
  std::array<double, 2> s_lim;
  std::array<double, 2> l_lim;
};

const std::vector<HslConfig> HSL_CONFIGS = {
  // Wide: typical default, all axes have substantial range.
  { "wide", { -360.0, 360.0 }, { 0.4, 1.0 }, { 0.3, 0.85 } },
  // Pinned saturation: zero-width on s.
  { "pinned_s", { 0.0, 360.0 }, { 0.7, 0.7 }, { 0.3, 0.7 } },
  // Pinned lightness: zero-width on l.
  { "pinned_l", { 0.0, 360.0 }, { 0.4, 1.0 }, { 0.5, 0.5 } },
  // Narrow saturation: very thin but nonzero — the "near-pinned" middle.
  { "narrow_s", { 0.0, 360.0 }, { 0.7, 0.71 }, { 0.3, 0.7 } },
};


struct Stats
{
  double min_de;
  double median_min_de;
  std::size_t n_candidates;
};

Stats
paletteStats(const std::vector<qualpal::colors::RGB>& palette)
{
  qualpal::metrics::CIEDE2000 metric;
  std::vector<qualpal::colors::Lab> lab;
  lab.reserve(palette.size());
  for (const auto& c : palette) {
    lab.emplace_back(c);
  }

  std::vector<double> mins;
  mins.reserve(lab.size());
  double overall = std::numeric_limits<double>::infinity();
  for (std::size_t i = 0; i < lab.size(); ++i) {
    double m = std::numeric_limits<double>::infinity();
    for (std::size_t j = 0; j < lab.size(); ++j) {
      if (i == j) {
        continue;
      }
      m = std::min(m, metric(lab[i], lab[j]));
    }
    mins.push_back(m);
    overall = std::min(overall, m);
  }
  std::sort(mins.begin(), mins.end());
  double median;
  if (mins.empty()) {
    median = 0.0;
  } else if (mins.size() % 2 == 1) {
    median = mins[mins.size() / 2];
  } else {
    median = 0.5 * (mins[mins.size() / 2 - 1] + mins[mins.size() / 2]);
  }
  return { overall, median, palette.size() };
}

// Old default: HSL-Halton in HSL parameters.
std::vector<qualpal::colors::RGB>
samplerHslHalton(const HslConfig& cfg, std::size_t n)
{
  return qualpal::hslColorGridDirect(cfg.h_lim, cfg.s_lim, cfg.l_lim, n);
}

// New default: race + top-up.
std::vector<qualpal::colors::RGB>
samplerHslViaLch(const HslConfig& cfg, std::size_t n)
{
  return qualpal::hslColorGridViaLch(cfg.h_lim, cfg.s_lim, cfg.l_lim, n);
}

struct Sampler
{
  const char* name;
  std::vector<qualpal::colors::RGB> (*fn)(const HslConfig&, std::size_t);
};

std::vector<qualpal::colors::RGB>
runQualpal(const std::vector<qualpal::colors::RGB>& candidates, std::size_t k)
{
  qualpal::Qualpal qp;
  qp.setInputRGB(candidates)
    .setMetric(qualpal::metrics::MetricType::CIEDE2000)
    .setMemoryLimit(32.0);
  return qp.generate(k);
}

} // namespace

int
main()
{
  const std::vector<std::size_t> n_points = { 500, 1000, 2000, 5000 };
  const std::vector<std::size_t> palette_sizes = { 5, 8 };

  const std::vector<Sampler> samplers = {
    { "hsl_halton_old", samplerHslHalton },
    { "hsl_via_lch", samplerHslViaLch },
  };

  std::cout << "config,sampler,n_points,k,actual_candidates,"
               "cand_ms,total_ms,min_de,median_min_de\n";

  for (const auto& cfg : HSL_CONFIGS) {
    for (const auto& s : samplers) {
      for (auto n : n_points) {
        auto cand_t0 = std::chrono::high_resolution_clock::now();
        auto candidates = s.fn(cfg, n);
        auto cand_t1 = std::chrono::high_resolution_clock::now();
        double cand_ms =
          std::chrono::duration<double, std::milli>(cand_t1 - cand_t0).count();

        for (auto k : palette_sizes) {
          if (candidates.size() < k) {
            continue;
          }
          auto t0 = std::chrono::high_resolution_clock::now();
          auto pal = runQualpal(candidates, k);
          auto t1 = std::chrono::high_resolution_clock::now();
          double total_ms =
            std::chrono::duration<double, std::milli>(t1 - t0).count();
          Stats st = paletteStats(pal);
          std::cout << cfg.name << "," << s.name << "," << n << "," << k << ","
                    << candidates.size() << "," << cand_ms << ","
                    << total_ms << "," << st.min_de << ","
                    << st.median_min_de << "\n";
        }
      }
    }
  }

  return 0;
}
