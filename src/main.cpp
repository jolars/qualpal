#include "qualpal/qualpal.h"
#include <CLI/CLI.hpp>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

// Check if the input is a valid HTML hex color representation
bool
isValidHexColor(const std::string& color)
{
  std::regex color_regex("^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$");
  return std::regex_match(color, color_regex);
}

std::array<double, 2>
splitHslString(const std::string& str)
{
  size_t pos = str.find(":");
  if (pos == std::string::npos) {
    throw std::invalid_argument("String does not contain a colon");
  }
  double first = std::stod(str.substr(0, pos));
  double second = std::stod(str.substr(pos + 1));
  return { first, second };
}

int
main(int argc, char** argv)
{
  CLI::App app{ "Qualitative color palettes" };
  // argv = app.ensure_utf8(argv);

  std::string input = "hex";

  app.add_option("-i,--input", input, "Type of input")
    ->check(CLI::IsMember({ "hex", "colorspace" }));

  int n = 8;
  std::vector<std::string> values;

  app.add_option<int, int>("-n,--number", n, "Number of colors to generate");
  app.add_option("values", values, "Colors or color space");

  CLI11_PARSE(app, argc, argv);

  if (values.empty()) {
    std::cout << "No value(s) provided" << std::endl;
    return 1;
  }

  std::vector<qualpal::RGB> rgb_out;

  if (input == "hex") {
    rgb_out = qualpal::qualpal(n, values);
  } else if (input == "colorspace") {
    if (values.size() != 3) {
      std::cout << "Three ranges (for hue, saturation, and lightness) are "
                   "required for the color space input"
                << std::endl;
      return 1;
    }
    std::array<double, 2> h_lim = splitHslString(values[0]);
    std::array<double, 2> s_lim = splitHslString(values[1]);
    std::array<double, 2> l_lim = splitHslString(values[2]);

    rgb_out = qualpal::qualpal(n, h_lim, s_lim, l_lim);
  }

  std::vector<std::string> hex_out;

  for (const auto& rgb : rgb_out) {
    hex_out.emplace_back(rgb.hex());
  }

  std::cout << "The generated colors are:" << std::endl;

  for (const auto& hex : hex_out) {
    std::cout << hex << std::endl;
  }

  return 0;
}
