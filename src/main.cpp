#include "qualpal/qualpal.h"
#include <CLI/CLI.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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
  CLI::App app{ "qualpal: automatic generation of qualitative color palettes\n"
                "A tool for creating perceptually distinct color palettes for "
                "data visualization." };

  app.require_subcommand(0, 1);

  std::string input = "hex";

  app
    .add_option("-i,--input",
                input,
                "Input type:\n"
                "  hex        - Hex color values (#ff0000)\n"
                "  colorspace - HSL ranges (h1:h2 s1:s2 l1:l2)\n"
                "  palette    - Built-in palette name")
    ->check(CLI::IsMember({ "hex", "colorspace", "palette" }));

  int n = 8;
  std::vector<std::string> values;

  app.add_option<int, int>("-n,--number", n, "Number of colors to generate");
  app.add_option("values", values, "Input values (depends on input type)")
    ->required();

  app.footer(
    "Examples:\n"
    "  Generate 5 colors from hex inputs:\n"
    "    qualpal -n 5 -i hex \"#ff0000\" \"#00ff00\" \"#0000ff\"\n\n"
    "  Generate palette from HSL ranges:\n"
    "    qualpal -n 8 -i colorspace \"0:360\" \"0.5:1\" \"0.3:0.7\"\n\n"
    "  Generate from built-in palette:\n"
    "    qualpal -n 3 -i palette \"ColorBrewer:Set1\"");

  auto help_cmd = app.add_subcommand("help", "Show detailed help information");

  argv = app.ensure_utf8(argv);
  CLI11_PARSE(app, argc, argv);

  if (values.empty()) {
    std::cerr << "Error: No values provided. Use --help for usage information."
              << std::endl;
    return 1;
  }

  std::vector<qualpal::RGB> rgb_out;

  try {
    if (input == "hex") {
      rgb_out = qualpal::qualpal(n, values);
    } else if (input == "colorspace") {
      if (values.size() != 3) {
        std::cerr << "Error: Colorspace input requires exactly 3 ranges (hue, "
                     "saturation, lightness)"
                  << std::endl;
        return 1;
      }
      std::array<double, 2> h_lim = splitHslString(values[0]);
      std::array<double, 2> s_lim = splitHslString(values[1]);
      std::array<double, 2> l_lim = splitHslString(values[2]);

      rgb_out = qualpal::qualpal(n, h_lim, s_lim, l_lim);
    } else if (input == "palette") {
      if (values.size() != 1) {
        std::cerr << "Error: Palette input requires exactly one palette name"
                  << std::endl;
        return 1;
      }
      rgb_out = qualpal::qualpal(n, values[0]);
    }
  } catch (const std::invalid_argument& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  std::vector<std::string> hex_out;

  for (const auto& rgb : rgb_out) {
    hex_out.emplace_back(rgb.hex());
  }

  for (const auto& hex : hex_out) {
    std::cout << hex << std::endl;
  }

  return 0;
}
