#include "qualpal/cvd_simulation.h"
#include "qualpal/palettes.h"
#include "qualpal/validation.h"
#include <CLI/CLI.hpp>
#include <algorithm>
#include <iostream>
#include <qualpal.h>
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

  double deutan = 0.0;
  double protan = 0.0;
  double tritan = 0.0;

  app.add_option(
    "--deutan", deutan, "Degree of deutan CVD simulation (0.0–1.0)");
  app.add_option(
    "--protan", protan, "Degree of protan CVD simulation (0.0–1.0)");
  app.add_option(
    "--tritan", tritan, "Degree of tritan CVD simulation (0.0–1.0)");

  int n = 8;
  std::vector<std::string> values;

  app.add_option<int, int>("-n,--number", n, "Number of colors to generate");
  app.add_option("values", values, "Input values (depends on input type)");

  app.footer(
    "Examples:\n"
    "  Generate 5 colors from hex inputs:\n"
    "    qualpal -n 5 -i hex \"#ff0000\" \"#00ff00\" \"#0000ff\"\n\n"
    "  Generate palette from HSL ranges:\n"
    "    qualpal -n 8 -i colorspace \"0:360\" \"0.5:1\" \"0.3:0.7\"\n\n"
    "  Generate from built-in palette:\n"
    "    qualpal -n 3 -i palette \"ColorBrewer:Set1\"");

  auto help_cmd = app.add_subcommand("help", "Show detailed help information");

  auto analyze_cmd = app.add_subcommand(
    "analyze", "Analyze a color palette by computing color difference matrix");

  std::string analyze_input = "hex";
  std::vector<std::string> analyze_values;
  double max_memory = 1.0;

  analyze_cmd
    ->add_option("-i,--input",
                 analyze_input,
                 "Input type:\n"
                 "  hex        - Hex color values (#ff0000)\n"
                 "  colorspace - HSL ranges (h1:h2 s1:s2 l1:l2)\n"
                 "  palette    - Built-in palette name")
    ->check(CLI::IsMember({ "hex", "colorspace", "palette" }));

  analyze_cmd
    ->add_option(
      "values", analyze_values, "Input values (depends on input type)")
    ->required();

  analyze_cmd->add_option(
    "--max-memory", max_memory, "Maximum memory usage in GB (default: 1.0)");

  argv = app.ensure_utf8(argv);
  CLI11_PARSE(app, argc, argv);

  std::map<std::string, double> cvd = { { "deutan", deutan },
                                        { "protan", protan },
                                        { "tritan", tritan } };

  if (*analyze_cmd) {
    if (analyze_values.empty()) {
      std::cerr << "Error: No values provided for analysis. Use --help for "
                   "usage information."
                << std::endl;
      return 1;
    }

    std::vector<qualpal::colors::RGB> rgb_colors;

    try {
      if (analyze_input == "hex") {
        for (const auto& color : analyze_values) {
          if (!qualpal::isValidHexColor(color)) {
            std::cerr << "Error: Invalid hex color '" << color
                      << "'. Expected format: #RRGGBB or #RGB" << std::endl;
            return 1;
          }
          rgb_colors.emplace_back(color);
        }
      } else if (analyze_input == "colorspace") {
        std::cerr << "Error: 'colorspace' input is not supported for analyze."
                  << std::endl;
        return 1;
      } else if (analyze_input == "palette") {
        if (analyze_values.size() != 1) {
          std::cerr << "Error: Palette input requires exactly one palette name"
                    << std::endl;
          return 1;
        }
        auto hex_colors = qualpal::getPalette(analyze_values[0]);
        for (const auto& hex : hex_colors) {
          rgb_colors.emplace_back(hex);
        }
      }
    } catch (const std::invalid_argument& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }

    auto original_rgb_colors = rgb_colors;

    for (const auto& cvd_pair : cvd) {
      const auto& cvd_type = cvd_pair.first;
      const auto& cvd_severity = cvd_pair.second;

      if (cvd_severity > 1.0 || cvd_severity < 0.0) {
        throw std::invalid_argument("cvd_severity must be between 0 and 1");
      }

      if (cvd_severity > 0) {
        std::transform(
          rgb_colors.begin(),
          rgb_colors.end(),
          rgb_colors.begin(),
          [&cvd_type, &cvd_severity](const qualpal::colors::RGB& rgb) {
            return qualpal::simulateCvd(rgb, cvd_type, cvd_severity);
          });
      }
    }

    // Compute and display color difference matrix
    try {
      auto matrix = qualpal::colorDifferenceMatrix(
        rgb_colors, qualpal::metrics::DIN99d{}, max_memory);

      std::cout << "Color Difference Matrix (DIN99d metric):\n";
      std::cout << "Colors analyzed: " << rgb_colors.size() << "\n";

      // Print color list
      std::cout << "Colors:\n";
      std::cout << "   " << std::setw(3) << "Idx" << std::setw(9) << "Hex"
                << std::setw(9) << "MinDist" << "\n";
      for (size_t i = 0; i < rgb_colors.size(); ++i) {
        double min_dist = std::numeric_limits<double>::max();
        for (size_t j = 0; j < rgb_colors.size(); ++j) {
          if (i != j) {
            min_dist = std::min(min_dist, matrix(i, j));
          }
        }

        // Extract RGB components for ANSI escape
        auto rgb = original_rgb_colors[i];
        int r = static_cast<int>(rgb.r() * 255);
        int g = static_cast<int>(rgb.g() * 255);
        int b = static_cast<int>(rgb.b() * 255);

        // Print color swatch using ANSI escape codes
        std::cout << "\033[48;2;" << r << ";" << g << ";" << b << "m  \033[0m "
                  << std::setw(3) << i << "  " << rgb.hex() << std::setw(9)
                  << std::fixed << std::setprecision(2) << min_dist << "\n";
      }
      std::cout << "\n";

      // Print matrix header
      std::cout << "Color Difference Matrix:\n";
      std::cout << "    ";
      for (size_t j = 0; j < rgb_colors.size(); ++j) {
        std::cout << std::setw(8) << j;
      }
      std::cout << "\n";

      // Print matrix rows
      for (size_t i = 0; i < rgb_colors.size(); ++i) {
        std::cout << std::setw(3) << i << " ";
        for (size_t j = 0; j < rgb_colors.size(); ++j) {
          std::cout << std::setw(8) << std::fixed << std::setprecision(2)
                    << matrix(i, j);
        }
        std::cout << "\n";
      }

    } catch (const std::runtime_error& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }

    return 0;
  }

  if (!*analyze_cmd && !*help_cmd) {
    if (values.empty()) {
      std::cerr
        << "Error: No values provided. Use --help for usage information."
        << std::endl;
      return 1;
    }
  }

  std::vector<qualpal::colors::RGB> rgb_out;

  try {
    if (input == "hex") {
      for (const auto& color : values) {
        if (!qualpal::isValidHexColor(color)) {
          std::cerr << "Error: Invalid hex color '" << color
                    << "'. Expected format: #RRGGBB or #RGB" << std::endl;
          return 1;
        }
      }
      rgb_out = qualpal::qualpal(n, values, cvd);
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

      qualpal::validateHslRanges(h_lim, s_lim, l_lim);

      rgb_out = qualpal::qualpal(n, h_lim, s_lim, l_lim, 1000, cvd);
    } else if (input == "palette") {
      if (values.size() != 1) {
        std::cerr << "Error: Palette input requires exactly one palette name"
                  << std::endl;
        return 1;
      }
      rgb_out = qualpal::qualpal(n, values[0], cvd);
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
