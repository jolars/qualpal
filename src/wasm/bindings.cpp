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
                                        qualpal::metrics::MetricType::DIN99d,
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

EMSCRIPTEN_BINDINGS(qualpal)
{
  class_<QualpalJS>("Qualpal")
    .constructor<>()
    .function("setInputRGB", &QualpalJS::setInputRGB)
    .function("setInputHex", &QualpalJS::setInputHex)
    .function("setInputPalette", &QualpalJS::setInputPalette)
    .function("setInputColorspace", &QualpalJS::setInputColorspace)
    .function("setCvd", &QualpalJS::setCvd)
    .function("setBackground", &QualpalJS::setBackground)
    .function("generate", &QualpalJS::generate)
    .function("extend", &QualpalJS::extend);

  // Free function binding
  // function("analyzePalette", &analyzePalette);
  function("analyzePalette", &analyzePaletteWrapper);
}
