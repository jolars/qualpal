#include <qualpal.h>

using namespace qualpal;

int
main()
{
  Qualpal qp;
  qp.setInputColorspace({ 0, 360 }, { 0.4, 0.8 }, { 0.3, 0.7 });
  auto colorspace_pal = qp.generate(5);

  // Select 2 colors from given RGB values
  qp.setInputRGB(
    { colors::RGB("#ff0000"), colors::RGB("#00ff00"), colors::RGB("#0000ff") });
  auto rgb_pal = qp.generate(2);

  // Consider color vision deficiency (CVD) when generating colors
  qp.setInputPalette("ColorBrewer:Set2").setCvd({ { "deutan", 0.7 } });
  auto cvd_pal = qp.generate(4);

  auto pal = Qualpal{}
               .setInputRGB({
                 colors::RGB("#f0f0f0"), // Light color (which we want to avoid)
                 colors::RGB("#e41a1c"), // Red
                 colors::RGB("#377eb8"), // Blue
                 colors::RGB("#4daf4a"), // Green
               })
               .setBackground(colors::RGB("#ffffff"))
               .generate(3);

  std::vector<colors::RGB> fixed = {
    colors::RGB("#e41a1c"), // Red
    colors::RGB("#377eb8"), // Blue
  };

  std::vector<colors::RGB> input = {
    colors::RGB("#4daf4a"), // Green
    colors::RGB("#984ea3"), // Purple
    colors::RGB("#ff7f00"), // Orange
    colors::RGB("#ffff33"), // Yellow
  };

  auto ext_pal = Qualpal{}.setInputRGB(input).extend(fixed, 4);

  return 0;
}
