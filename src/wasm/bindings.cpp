#include "../src/qualpal/cvd.h"
#include "../src/qualpal/palettes.h"
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <map>
#include <qualpal.h>
#include <string>
#include <vector>

using namespace emscripten;

// Wrapper class to make the API more JavaScript-friendly
class QualpalJS
{
private:
  qualpal::Qualpal qp;

public:
  QualpalJS() = default;

  void setInputRGB(const val& colors_array)
  {
    std::vector<qualpal::colors::RGB> rgb_colors;
    int length = colors_array["length"].as<int>();

    for (int i = 0; i < length; ++i) {
      val color = colors_array[i];
      double r = color["r"].as<double>();
      double g = color["g"].as<double>();
      double b = color["b"].as<double>();
      rgb_colors.emplace_back(r, g, b);
    }
    qp.setInputRGB(rgb_colors);
  }

  void setInputHex(const val& hex_array)
  {
    std::vector<std::string> hex_colors;
    int length = hex_array["length"].as<int>();

    for (int i = 0; i < length; ++i) {
      hex_colors.push_back(hex_array[i].as<std::string>());
    }
    qp.setInputHex(hex_colors);
  }

  void setInputPalette(const std::string& palette_name)
  {
    qp.setInputPalette(palette_name);
  }

  void setInputColorspace(double h_min,
                          double h_max,
                          double s_min,
                          double s_max,
                          double l_min,
                          double l_max)
  {
    qp.setInputColorspace({ h_min, h_max }, { s_min, s_max }, { l_min, l_max });
  }

  void setInputColorspaceRegions(const val& regions_array,
                                 const std::string& space_type)
  {
    std::vector<qualpal::ColorspaceRegion> regions;
    int length = regions_array["length"].as<int>();

    for (int i = 0; i < length; ++i) {
      val region = regions_array[i];
      qualpal::ColorspaceRegion r;
      r.h_lim = { region["h_min"].as<double>(), region["h_max"].as<double>() };
      r.s_or_c_lim = { region["s_or_c_min"].as<double>(),
                       region["s_or_c_max"].as<double>() };
      r.l_lim = { region["l_min"].as<double>(), region["l_max"].as<double>() };
      regions.push_back(r);
    }

    qualpal::ColorspaceType space = qualpal::ColorspaceType::HSL;
    if (space_type == "LCHab") {
      space = qualpal::ColorspaceType::LCHab;
    }

    qp.setInputColorspaceRegions(regions, space);
  }

  void setCvd(const val& cvd_obj)
  {
    std::map<std::string, double> cvd_params;
    val keys = val::global("Object").call<val>("keys", cvd_obj);
    int length = keys["length"].as<int>();

    for (int i = 0; i < length; ++i) {
      std::string key = keys[i].as<std::string>();
      double value = cvd_obj[key].as<double>();
      cvd_params[key] = value;
    }
    qp.setCvd(cvd_params);
  }

  void setBackground(double r, double g, double b)
  {
    qp.setBackground(qualpal::colors::RGB(r, g, b));
  }

  void setWhitePoint(const std::string& wp_name)
  {
    qualpal::WhitePoint wp;
    if (wp_name == "D65") {
      wp = qualpal::WhitePoint::D65;
    } else if (wp_name == "D50") {
      wp = qualpal::WhitePoint::D50;
    } else if (wp_name == "D55") {
      wp = qualpal::WhitePoint::D55;
    } else if (wp_name == "A") {
      wp = qualpal::WhitePoint::A;
    } else if (wp_name == "E") {
      wp = qualpal::WhitePoint::E;
    } else {
      wp = qualpal::WhitePoint::D65; // Default
    }
    qp.setWhitePoint(wp);
  }

  val generate(int n)
  {
    auto palette = qp.generate(n);
    val result = val::array();

    for (size_t i = 0; i < palette.size(); ++i) {
      val color = val::object();
      color.set("hex", palette[i].hex());
      color.set("r", palette[i].r());
      color.set("g", palette[i].g());
      color.set("b", palette[i].b());
      result.call<void>("push", color);
    }

    return result;
  }

  val extend(const val& existing_palette_array, int n)
  {
    // Convert existing palette from JS array
    std::vector<qualpal::colors::RGB> existing_palette;
    int length = existing_palette_array["length"].as<int>();

    for (int i = 0; i < length; ++i) {
      val color = existing_palette_array[i];
      double r = color["r"].as<double>();
      double g = color["g"].as<double>();
      double b = color["b"].as<double>();
      existing_palette.emplace_back(r, g, b);
    }

    auto palette = qp.extend(existing_palette, n);
    val result = val::array();

    for (size_t i = 0; i < palette.size(); ++i) {
      val color = val::object();
      color.set("hex", palette[i].hex());
      color.set("r", palette[i].r());
      color.set("g", palette[i].g());
      color.set("b", palette[i].b());
      result.call<void>("push", color);
    }

    return result;
  }
};

// Wrapper function that uses only JavaScript-compatible types
val
analyzePaletteWrapper(const val& colors_array,
                      const val& cvd_obj,
                      const val& bg_obj,
                      double max_memory)
{
  // Convert colors array
  std::vector<qualpal::colors::RGB> rgb_colors;
  int length = colors_array["length"].as<int>();
  for (int i = 0; i < length; ++i) {
    val color = colors_array[i];
    double r = color["r"].as<double>() / 255;
    double g = color["g"].as<double>() / 255;
    double b = color["b"].as<double>() / 255;
    rgb_colors.emplace_back(r, g, b);
  }

  // Convert cvd object to map
  std::map<std::string, double> cvd_params;
  if (!cvd_obj.isNull() && !cvd_obj.isUndefined()) {
    val keys = val::global("Object").call<val>("keys", cvd_obj);
    int keys_length = keys["length"].as<int>();
    for (int i = 0; i < keys_length; ++i) {
      std::string key = keys[i].as<std::string>();
      double value = cvd_obj[key].as<double>();
      cvd_params[key] = value;
    }
  }

  // Convert background color to optional
  std::optional<qualpal::colors::RGB> bg;
  if (!bg_obj.isNull() && !bg_obj.isUndefined()) {
    double r = bg_obj["r"].as<double>();
    double g = bg_obj["g"].as<double>();
    double b = bg_obj["b"].as<double>();
    bg = qualpal::colors::RGB(r, g, b);
  }

  // Call the actual analyze function
  auto result = qualpal::analyzePalette(rgb_colors,
                                        qualpal::metrics::MetricType::CIEDE2000,
                                        cvd_params,
                                        bg,
                                        max_memory);

  // Convert result to JavaScript object
  val js_result = val::object();

  for (const auto& [cvd_type, analysis] : result) {
    val analysis_obj = val::object();

    // Convert difference_matrix to JS array of arrays
    val matrix = val::array();

    for (size_t i = 0; i < analysis.difference_matrix.nrow(); ++i) {
      val row = val::array();
      for (size_t j = 0; j < analysis.difference_matrix.ncol(); ++j) {
        row.set(j, analysis.difference_matrix(i, j));
      }
      matrix.set(i, row);
    }

    analysis_obj.set("differenceMatrix", matrix);

    // Min distances
    val min_distances = val::array();

    for (size_t i = 0; i < analysis.min_distances.size(); ++i) {
      min_distances.set(i, analysis.min_distances[i]);
    }

    analysis_obj.set("minDistances", min_distances);
    analysis_obj.set("bgMinDistance", analysis.bg_min_distance);
    js_result.set(cvd_type, analysis_obj);
  }
  return js_result;
}

// Wrapper: list available palettes (domain -> palettes[])
val
listAvailablePalettesWrapper()
{
  auto mp = qualpal::listAvailablePalettes();
  val out = val::object();
  for (const auto& kv : mp) {
    const auto& domain = kv.first;
    const auto& palettes = kv.second;
    val arr = val::array();
    for (std::size_t i = 0; i < palettes.size(); ++i) {
      arr.set(i, palettes[i]);
    }
    out.set(domain, arr);
  }
  return out;
}

// Wrapper: get palette hex list by domain + name
val
getPaletteWrapper(const std::string& domain, const std::string& palette)
{
  auto hex = qualpal::getPalette(domain + ":" + palette);
  val arr = val::array();
  for (std::size_t i = 0; i < hex.size(); ++i) {
    arr.set(i, hex[i]);
  }
  return arr;
}

val
simulateCvdWrapper(double r,
                   double g,
                   double b,
                   const std::string& cvd_type,
                   double severity)
{
  auto simulated =
    qualpal::simulateCvd(qualpal::colors::RGB(r, g, b), cvd_type, severity);

  val result = val::object();
  result.set("r", simulated.r());
  result.set("g", simulated.g());
  result.set("b", simulated.b());
  result.set("hex", simulated.hex());
  return result;
}

EMSCRIPTEN_BINDINGS(qualpal)
{
  class_<QualpalJS>("Qualpal")
    .constructor<>()
    .function("setInputRGB", &QualpalJS::setInputRGB)
    .function("setInputHex", &QualpalJS::setInputHex)
    .function("setInputPalette", &QualpalJS::setInputPalette)
    .function("setInputColorspace", &QualpalJS::setInputColorspace)
    .function("setInputColorspaceRegions",
              &QualpalJS::setInputColorspaceRegions)
    .function("setCvd", &QualpalJS::setCvd)
    .function("setBackground", &QualpalJS::setBackground)
    .function("setWhitePoint", &QualpalJS::setWhitePoint)
    .function("generate", &QualpalJS::generate)
    .function("extend", &QualpalJS::extend);

  // Free function binding
  function("analyzePalette", &analyzePaletteWrapper);
  function("listAvailablePalettes", &listAvailablePalettesWrapper);
  function("getPaletteHex", &getPaletteWrapper);
  function("simulateCvd", &simulateCvdWrapper);
}
