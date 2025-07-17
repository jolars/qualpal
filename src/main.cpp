#include "qualpal/cvd.h"
#include "qualpal/palettes.h"
#include "qualpal/validation.h"
#include <CLI/CLI.hpp>
#include <algorithm>
#include <iostream>
#include <qualpal.h>
#include <stdexcept>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <io.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif

bool
is_stdout_tty()
{
  return ISATTY(FILENO(stdout));
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

#if defined(_WIN32)
#include <windows.h>
#undef RGB
void
enable_ansi_escape()
{
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &dwMode)) {
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
  }
}
#endif

int
main(int argc, char** argv)
{
  CLI::App app{ "qualpal: automatic generation of qualitative color palettes\n"
                "A tool for creating perceptually distinct color palettes for "
                "data visualization." };

  app.require_subcommand(0, 1);

  std::string input = "hex";
  std::string output_delim = "newline";
  std::string colorize = "auto";

  app
    .add_option("-i,--input",
                input,
                "Input type:\n"
                "  hex        - Hex color values (#ff0000)\n"
                "  colorspace - HSL ranges (h1:h2 s1:s2 l1:l2)\n"
                "  palette    - Built-in palette name")
    ->check(CLI::IsMember({ "hex", "colorspace", "palette" }));

  app
    .add_option("--output-delim",
                output_delim,
                "Delimiter for output: newline (default), space, comma")
    ->check(CLI::IsMember({ "newline", "space", "comma" }));

  app
    .add_option("--colorize",
                colorize,
                "Colorize hex output: auto (default), always, never")
    ->check(CLI::IsMember({ "auto", "always", "never" }));

  std::string background = "";

  app.add_option(
    "-b,--background", background, "Background color in hex (e.g. #ffffff)");

  int n_colors = 1000;

  app.add_option(
    "-p,--points",
    n_colors,
    "Number of candidate points for colorspace search (default: 1000)");

  std::string metric_str = "din99d";

  app
    .add_option("-m,--metric",
                metric_str,
                "Color difference metric to use:\n"
                "  din99d      - Perceptual color difference (default)\n"
                "  ciede2000   - CIEDE2000 color difference\n"
                "  cie76       - CIE76 color difference")
    ->check(CLI::IsMember({ "din99d", "ciede2000", "cie76" }));

  double max_memory = 1.0;

  app.add_option(
    "--max-memory", max_memory, "Maximum memory usage in GB (default: 1.0)");

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

  auto list_cmd =
    app.add_subcommand("list-palettes", "List all available built-in palettes");

  std::string analyze_input = "hex";
  std::vector<std::string> analyze_values;

  analyze_cmd
    ->add_option("-i,--input",
                 analyze_input,
                 "Input type:\n"
                 "  hex        - Hex color values (#ff0000)\n"
                 "  palette    - Built-in palette name")
    ->check(CLI::IsMember({ "hex", "colorspace", "palette" }));

  analyze_cmd
    ->add_option(
      "values", analyze_values, "Input values (depends on input type)")
    ->required();

  argv = app.ensure_utf8(argv);
  CLI11_PARSE(app, argc, argv);

  qualpal::metrics::MetricType metric;
  if (metric_str == "din99d") {
    metric = qualpal::metrics::MetricType::DIN99d;
  } else if (metric_str == "ciede2000") {
    metric = qualpal::metrics::MetricType::CIEDE2000;
  } else if (metric_str == "cie76") {
    metric = qualpal::metrics::MetricType::CIE76;
  } else {
    throw std::invalid_argument("Unknown metric: " + metric_str);
  }

  std::map<std::string, double> cvd = { { "deutan", deutan },
                                        { "protan", protan },
                                        { "tritan", tritan } };

  qualpal::Qualpal qp;

  if (*list_cmd) {
    auto palettes = qualpal::listAvailablePalettes();
    std::cout << "Available palettes:\n";
    for (const auto& pkg_pair : palettes) {
      std::cout << "  " << pkg_pair.first << ":\n";
      for (const auto& pal : pkg_pair.second) {
        std::cout << "    " << pal << "\n";
      }
    }
    return 0;
  }

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
    if (!background.empty()) {
      if (!qualpal::isValidHexColor(background)) {
        std::cerr << "Error: Invalid background color '" << background
                  << "'. Expected format: #RRGGBB or #RGB" << std::endl;
        return 1;
      }
      auto bg = qualpal::colors::RGB(background);
      qp.setBackground(bg);
    }

    qp.setCvd(cvd);
    qp.setMetric(metric);
    qp.setMemoryLimit(max_memory);
    qp.setColorspaceSize(n_colors);

    if (input == "hex") {
      for (const auto& color : values) {
        if (!qualpal::isValidHexColor(color)) {
          std::cerr << "Error: Invalid hex color '" << color
                    << "'. Expected format: #RRGGBB or #RGB" << std::endl;
          return 1;
        }
      }
      qp.setInputHex(values);
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

      qp.setInputColorspace(h_lim, s_lim, l_lim);
    } else if (input == "palette") {
      if (values.size() != 1) {
        std::cerr << "Error: Palette input requires exactly one palette name"
                  << std::endl;
        return 1;
      }
      qp.setInputPalette(values[0]);
    }
    rgb_out = qp.generate(n);
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

  bool use_color = false;
  if (colorize == "always") {
    use_color = true;
  } else if (colorize == "auto") {
    use_color = isatty(fileno(stdout));
  }

#if defined(_WIN32)
  if (use_color) {
    enable_ansi_escape();
  }
#endif

  std::string delim;
  if (output_delim == "space") {
    delim = " ";
  } else if (output_delim == "comma") {
    delim = ",";
  } else {
    delim = "\n";
  }

  for (size_t i = 0; i < hex_out.size(); ++i) {
    if (use_color) {
      // Parse hex to RGB for colorizing
      const std::string& hex = hex_out[i];
      int r = std::stoi(hex.substr(1, 2), nullptr, 16);
      int g = std::stoi(hex.substr(3, 2), nullptr, 16);
      int b = std::stoi(hex.substr(5, 2), nullptr, 16);
      // Compute luminance for contrast
      double lum = 0.2126 * r + 0.7152 * g + 0.0722 * b;
      int fg = (lum > 128) ? 0 : 255; // black text on bright, white on dark
      std::cout << "\033[48;2;" << r << ";" << g << ";" << b << "m"
                << "\033[38;2;" << fg << ";" << fg << ";" << fg << "m" << hex
                << "\033[0m";
    } else {
      std::cout << hex_out[i];
    }
    if (i + 1 < hex_out.size())
      std::cout << delim;
  }
  if (delim != "\n")
    std::cout << std::endl;

  return 0;
}
