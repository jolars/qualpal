// Multi-start refinement benchmark.
//
// Compares discrete-only, single-start refinement (the current default),
// and multi-start refinement (production refinement run from seed 0 plus
// M random-start refinements, take the best). Reports the bound from a
// matched-cost aggressive multi-start as a quality ceiling.
//
// Build: cmake -B build -S . -DBUILD_TUNING=ON && cmake --build build
// Run:   ./build/tools/multistart_bench > multistart.csv

#include "../src/qualpal/color_grid.h"
#include "../src/qualpal/continuous_refinement.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <qualpal.h>
#include <random>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace qualpal;

namespace {

const std::array<double, 3> WP_D65 = { 0.95047, 1.0, 1.08883 };

struct Cfg
{
  const char* name;
  std::array<double, 2> h, s, l;
};

const std::vector<Cfg> CFGS = {
  { "wide", { 0, 360 }, { 0.4, 1.0 }, { 0.3, 0.85 } },
  { "narrow_s", { 0, 360 }, { 0.7, 0.71 }, { 0.3, 0.7 } },
};

double
minDeltaE(const std::vector<colors::RGB>& pal)
{
  metrics::CIEDE2000 dE;
  std::vector<colors::Lab> lab;
  for (auto& c : pal)
    lab.emplace_back(c);
  double m = std::numeric_limits<double>::infinity();
  for (size_t i = 0; i < lab.size(); ++i)
    for (size_t j = i + 1; j < lab.size(); ++j)
      m = std::min(m, dE(lab[i], lab[j]));
  return m;
}

bool
inGamutLinear(const colors::Lab& lab, const std::array<double, 3>& wp)
{
  colors::XYZ xyz(lab, wp);
  double x = xyz.x(), y = xyz.y(), z = xyz.z();
  double r = 3.2404542 * x - 1.5371385 * y - 0.4985314 * z;
  double g = -0.9692660 * x + 1.8760108 * y + 0.0415560 * z;
  double b = 0.0556434 * x - 0.2040259 * y + 1.0572252 * z;
  constexpr double eps = 1e-9;
  return r >= -eps && r <= 1 + eps && g >= -eps && g <= 1 + eps &&
         b >= -eps && b <= 1 + eps;
}

bool
inHsl(const colors::Lab& lab, const Cfg& cfg)
{
  colors::HSL h(lab);
  bool sl = h.s() >= cfg.s[0] && h.s() <= cfg.s[1] && h.l() >= cfg.l[0] &&
            h.l() <= cfg.l[1];
  if (!sl)
    return false;
  for (double off : { -360.0, 0.0, 360.0 }) {
    double hh = h.h() + off;
    if (hh >= cfg.h[0] && hh <= cfg.h[1])
      return true;
  }
  return false;
}

// Sample k random in-region, in-gamut Lab points to seed a refinement run.
std::vector<colors::XYZ>
randomStartXYZ(std::mt19937& rng, const Cfg& cfg, size_t k)
{
  std::uniform_real_distribution<double> hd(cfg.h[0], cfg.h[1]),
    sd(cfg.s[0], cfg.s[1]), ld(cfg.l[0], cfg.l[1]);
  std::vector<colors::XYZ> out;
  out.reserve(k);
  while (out.size() < k) {
    double h = hd(rng);
    if (h < 0)
      h += 360;
    if (h >= 360)
      h -= 360;
    colors::HSL hsl(h, sd(rng), ld(rng));
    colors::Lab lab(hsl);
    if (!inGamutLinear(lab, WP_D65))
      continue;
    if (!inHsl(lab, cfg))
      continue;
    out.emplace_back(lab, WP_D65);
  }
  return out;
}

std::vector<ColorspaceRegion>
regionsFor(const Cfg& cfg)
{
  return { ColorspaceRegion{ cfg.h, cfg.s, cfg.l } };
}

// Run the standard production pipeline (discrete farthestPoints + refine).
std::vector<colors::RGB>
standardPipeline(const Cfg& cfg, size_t n_points, size_t k, bool refine)
{
  Qualpal qp;
  qp.setInputColorspace(cfg.h, cfg.s, cfg.l)
    .setColorspaceSize(n_points)
    .setMetric(metrics::MetricType::CIEDE2000)
    .setRefinementStarts(refine ? 1 : 0)
    .setMemoryLimit(32.0);
  return qp.generate(k);
}

// Random start + production refinement.
std::vector<colors::RGB>
randomThenRefine(std::mt19937& rng, const Cfg& cfg, size_t k)
{
  auto seed_xyz = randomStartXYZ(rng, cfg, k);
  auto refined = refinePalette(std::move(seed_xyz),
                               0,
                               false,
                               regionsFor(cfg),
                               ColorspaceType::HSL,
                               WP_D65,
                               {});
  std::vector<colors::RGB> out;
  out.reserve(k);
  for (auto& xyz : refined.selected)
    out.emplace_back(xyz);
  return out;
}

double
runMultistart(const Cfg& cfg,
              size_t n_points,
              size_t k,
              size_t n_extra_starts,
              uint64_t seed)
{
  // Seed 0: production pipeline (warm start from discrete farthestPoints).
  auto pal0 = standardPipeline(cfg, n_points, k, true);
  double best = minDeltaE(pal0);

  std::vector<double> per_start(n_extra_starts, 0.0);
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)
#endif
  for (long long s = 0; s < (long long)n_extra_starts; ++s) {
    std::mt19937 rng(seed + s + 1);
    auto pal = randomThenRefine(rng, cfg, k);
    per_start[s] = minDeltaE(pal);
  }
  for (double v : per_start)
    best = std::max(best, v);
  return best;
}

} // namespace

int
main()
{
  const std::vector<size_t> ks = { 5, 8 };
  const size_t n_points = 2000;
  const std::vector<size_t> ms = { 0, 4, 9, 24 };
  const uint64_t seed = 1234;

  std::cout << "config,k,n_points,strategy,extra_starts,total_ms,min_de\n";

  for (const auto& cfg : CFGS) {
    for (size_t k : ks) {
      // Discrete-only baseline.
      {
        auto t0 = std::chrono::high_resolution_clock::now();
        auto pal = standardPipeline(cfg, n_points, k, false);
        auto t1 = std::chrono::high_resolution_clock::now();
        double ms_ =
          std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << cfg.name << "," << k << "," << n_points
                  << ",discrete_only,0," << ms_ << "," << minDeltaE(pal)
                  << "\n";
      }
      // Single-start refined (current commit).
      {
        auto t0 = std::chrono::high_resolution_clock::now();
        auto pal = standardPipeline(cfg, n_points, k, true);
        auto t1 = std::chrono::high_resolution_clock::now();
        double ms_ =
          std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << cfg.name << "," << k << "," << n_points
                  << ",single_start,0," << ms_ << "," << minDeltaE(pal)
                  << "\n";
      }
      // Multi-start: production seed + M random starts.
      for (size_t M : ms) {
        if (M == 0)
          continue;
        auto t0 = std::chrono::high_resolution_clock::now();
        double best = runMultistart(cfg, n_points, k, M, seed);
        auto t1 = std::chrono::high_resolution_clock::now();
        double ms_ =
          std::chrono::duration<double, std::milli>(t1 - t0).count();
        std::cout << cfg.name << "," << k << "," << n_points << ",multistart,"
                  << M << "," << ms_ << "," << best << "\n";
      }
    }
  }
}
