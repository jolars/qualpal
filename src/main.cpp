#include "qualpal/qualpal.h"
#include <CLI/CLI.hpp>
#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

// Check if the input is a valid HTML hex color representation
bool
isValidHexColor(const std::string& color)
{
  std::regex color_regex("^#([A-Fa-f0-9]{6}|[A-Fa-f0-9]{3})$");
  return std::regex_match(color, color_regex);
}

int
main(int argc, char** argv)
{
  CLI::App app{ "Qualitative color palettes" };
  // argv = app.ensure_utf8(argv);

  std::string input = "hex";

  app.add_option("-i,--input", input, "Type of input")
    ->check(CLI::IsMember({ "hex" }));

  std::vector<std::string> values;

  app.add_option("values", values, "Type of input");

  CLI11_PARSE(app, argc, argv);

  if (values.empty()) {
    std::cout << "No value(s) provided" << std::endl;
    return 1;
  }

  if (input == "hex") {
    std::vector<qualpal::RGB> rgb_colors;
    for (auto hex : values) {
      assert(isValidHexColor(hex) && "Values are hex colors");

      rgb_colors.emplace_back(hex);
    }
  }
  return 0;
}
