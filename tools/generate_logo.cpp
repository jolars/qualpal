#include <qualpal/qualpal.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <locale>
#include <string_view>
#include <vector>

int
main(int argc, char** argv)
{
  if (argc != 1 && !(argc == 2 && std::string_view(argv[1]) == "--json")) {
    std::cerr << "Usage: generate_logo [--json]\n";
    return 1;
  }

  std::vector<qualpal::colors::RGB> candidates;
  for (int hue = 0; hue < 360; hue += 15) {
    for (const double lightness : { 0.36, 0.49, 0.62 }) {
      for (const double saturation : { 0.48, 0.93 }) {
        candidates.emplace_back(
          qualpal::colors::HSL(hue, saturation, lightness));
      }
    }
  }

  const auto selected = qualpal::Qualpal{}
                          .setInputRGB(candidates)
                          .setMetric(qualpal::metrics::MetricType::DIN99d)
                          .generate(5);

  std::cout.imbue(std::locale::classic());
  std::cout << std::setprecision(17) << std::boolalpha << "[\n";
  for (auto it = candidates.begin(); it != candidates.end(); ++it) {
    const qualpal::colors::DIN99d color(*it);
    const bool is_selected =
      std::find(selected.begin(), selected.end(), *it) != selected.end();
    std::cout << (it == candidates.begin() ? "  " : ",\n  ")
              << "{\"coordinates\":[" << color.a() << ',' << color.b() << ','
              << color.l() << "],\"hex\":\"" << it->hex()
              << "\",\"selected\":" << is_selected << '}';
  }
  std::cout << "\n]\n";
}
