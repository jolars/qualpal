/**
 * @file
 * @brief Color difference metrics for qualpal
 *
 * This file contains various color difference calculation methods that can be
 * used with the colorDifferenceMatrix function to compute perceptual color
 * differences.
 */

#pragma once

#include <cmath>
#include <qualpal_bits/colors.h>

namespace qualpal {
namespace metrics {

// Forward declarations
class RGB;
class HSL;
class XYZ;
class Lab;
class DIN99d;

/**
 * @brief DIN99d color difference with optional power transformation
 *
 * Calculates Euclidean distance in DIN99d color space, with optional
 * power transformation for improved perceptual uniformity.
 */
class DIN99d
{
private:
  bool use_power_transform;
  double power;
  double scale;

public:
  /**
   * @brief Construct DIN99d metric with configurable parameters
   * @param use_power_transform Whether to apply power transformation (default:
   * true)
   * @param power Power value for transformation (default: 0.74)
   * @param scale Scale factor after transformation (default: 1.28)
   */
  explicit DIN99d(bool use_power_transform = true,
                  double power = 0.74,
                  double scale = 1.28)
    : use_power_transform(use_power_transform)
    , power(power)
    , scale(scale)
  {
  }

  /**
   * @brief Calculate color difference between two colors
   * @tparam ColorType Any color class that can be converted to colors::DIN99d
   * @param c1 First color
   * @param c2 Second color
   * @return Color difference value (Delta E in DIN99d space)
   */
  template<typename ColorType>
  double operator()(const ColorType& c1, const ColorType& c2) const
  {
    qualpal::DIN99d d1(c1), d2(c2);
    double d =
      std::sqrt(std::pow(d1.l() - d2.l(), 2) + std::pow(d1.a() - d2.a(), 2) +
                std::pow(d1.b() - d2.b(), 2));

    if (use_power_transform) {
      return std::pow(d, power) * scale;
    } else {
      return d;
    }
  }
};

/**
 * @brief CIE76 (Delta E 1976) color difference
 *
 * Standard CIE76 color difference formula using Euclidean distance in CIE Lab
 * space.
 */
struct CIE76
{
  /**
   * @brief Calculate CIE76 color difference
   * @tparam ColorType Any color class that can be converted to colors::Lab
   * @param c1 First color
   * @param c2 Second color
   * @return CIE76 Delta E value
   */
  template<typename ColorType>
  double operator()(const ColorType& c1, const ColorType& c2) const
  {
    qualpal::Lab l1(c1), l2(c2);
    return std::sqrt(std::pow(l1.l() - l2.l(), 2) +
                     std::pow(l1.a() - l2.a(), 2) +
                     std::pow(l1.b() - l2.b(), 2));
  }
};

} // namespace metrics
} // namespace qualpal
