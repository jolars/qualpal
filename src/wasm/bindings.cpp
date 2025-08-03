#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <map>
#include <qualpal.h>
#include <string>
#include <vector>

using namespace emscripten;
using namespace qualpal;

// Wrapper class to make the API more JavaScript-friendly
class QualpalJS
{
private:
  Qualpal qp;

public:
  QualpalJS() = default;

  void setInputRGB(const val& colors_array)
  {
    std::vector<colors::RGB> rgb_colors;
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
    qp.setBackground(colors::RGB(r, g, b));
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
};

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
    .function("generate", &QualpalJS::generate);
}
