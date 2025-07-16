#include "../qualpal/color_grid.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <qualpal/colors.h>
#include <qualpal/qualpal.h>

void
generateHSLData()
{
  // Generate HSL color grid similar to qualpal algorithm
  auto hsl_colors =
    qualpal::colorGrid({ -200, 120 }, { 0.3, 0.8 }, { 0.4, 0.9 }, 1000);

  std::ofstream file("data/hsl_points.dat");
  file << R"(# H S L RGB_HEX
)";

  for (const auto& hsl : hsl_colors) {
    qualpal::colors::RGB rgb(hsl);
    int r = static_cast<int>(rgb.r() * 255);
    int g = static_cast<int>(rgb.g() * 255);
    int b = static_cast<int>(rgb.b() * 255);
    int rgb_hex = (r << 16) | (g << 8) | b;
    file << std::fixed << std::setprecision(6) << hsl.h() << " " << hsl.s()
         << " " << hsl.l() << " " << rgb_hex << "\n";
  }
  file.close();
}

void
generateRGBProjection()
{
  auto hsl_colors =
    qualpal::colorGrid({ -200, 120 }, { 0.3, 0.8 }, { 0.4, 0.9 }, 500);

  std::ofstream file("data/rgb_points.dat");
  file << "# R G B H S L RGB_HEX\n";

  for (const auto& hsl : hsl_colors) {
    qualpal::colors::RGB rgb(hsl);
    int r = static_cast<int>(rgb.r() * 255);
    int g = static_cast<int>(rgb.g() * 255);
    int b = static_cast<int>(rgb.b() * 255);
    int rgb_hex = (r << 16) | (g << 8) | b;
    file << std::fixed << std::setprecision(6) << rgb.r() * 255 << " "
         << rgb.g() * 255 << " " << rgb.b() * 255 << " " << hsl.h() << " "
         << hsl.s() << " " << hsl.l() << " " << rgb_hex << "\n";
  }
  file.close();
}

void
generateDIN99dProjection()
{
  auto hsl_colors =
    qualpal::colorGrid({ -200, 120 }, { 0.3, 0.8 }, { 0.4, 0.9 }, 500);

  std::ofstream file("data/din99d_points.dat");
  file << "# L_din a_din b_din RGB_HEX\n";

  for (const auto& hsl : hsl_colors) {
    qualpal::colors::RGB rgb(hsl);
    qualpal::colors::DIN99d din(rgb);
    int r = static_cast<int>(rgb.r() * 255);
    int g = static_cast<int>(rgb.g() * 255);
    int b = static_cast<int>(rgb.b() * 255);
    int rgb_hex = (r << 16) | (g << 8) | b;
    file << std::fixed << std::setprecision(6) << din.l() << " " << din.a()
         << " " << din.b() << " " << rgb_hex << "\n";
  }
  file.close();
}

void
generateSelectedColors()
{
  // Generate the full color space
  auto hsl_colors =
    qualpal::colorGrid({ -200, 120 }, { 0.3, 0.8 }, { 0.4, 0.9 }, 1000);
  std::vector<qualpal::colors::RGB> rgb_colors;
  for (const auto& hsl : hsl_colors) {
    rgb_colors.emplace_back(hsl);
  }

  // Select 5 colors using qualpal
  auto selected = qualpal::Qualpal{}.setInputRGB(rgb_colors).generate(5);

  // Write all points
  std::ofstream all_file("data/all_colors.dat");
  all_file << "# L_din a_din b_din RGB_HEX selected\n";

  std::ofstream selected_file("data/selected_colors.dat");
  selected_file << "# L_din a_din b_din RGB_HEX\n";

  for (const auto& rgb : rgb_colors) {
    qualpal::colors::DIN99d din(rgb);
    bool is_selected = false;

    // Check if this color was selected
    for (const auto& sel : selected) {
      if (std::abs(rgb.r() - sel.r()) < 1e-6 &&
          std::abs(rgb.g() - sel.g()) < 1e-6 &&
          std::abs(rgb.b() - sel.b()) < 1e-6) {
        is_selected = true;
        break;
      }
    }

    int r = static_cast<int>(rgb.r() * 255);
    int g = static_cast<int>(rgb.g() * 255);
    int b = static_cast<int>(rgb.b() * 255);
    int rgb_hex = (r << 16) | (g << 8) | b;
    all_file << std::fixed << std::setprecision(6) << din.l() << " " << din.a()
             << " " << din.b() << " " << rgb_hex << " " << (is_selected ? 1 : 0)
             << "\n";

    if (is_selected) {
      selected_file << std::fixed << std::setprecision(6) << din.l() << " "
                    << din.a() << " " << din.b() << " " << rgb_hex << "\n";
    }
  }

  all_file.close();
  selected_file.close();
}

int
main()
{
  std::cout << "Generating HSL data..." << std::endl;
  generateHSLData();

  std::cout << "Generating RGB projection..." << std::endl;
  generateRGBProjection();

  std::cout << "Generating DIN99d projection..." << std::endl;
  generateDIN99dProjection();

  std::cout << "Generating selected colors..." << std::endl;
  generateSelectedColors();

  std::cout << "Data generation complete!" << std::endl;
  return 0;
}
