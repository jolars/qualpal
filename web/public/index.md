# Qualpal

[Qualpal](https://qualpal.cc/) generates qualitative color palettes for categorical
data. It maximizes the minimum perceived color difference to create distinct
colors for data visualization.

## Generate a palette

1. Choose a palette size from 2 to 12 colors.
2. Select an input mode: Colorspace or Fixed Set.
3. In Colorspace mode, configure hue, saturation, and lightness ranges. Add
   regions to combine multiple parts of the HSL color space.
4. In Fixed Set mode, supply hexadecimal colors or select a built-in palette.
5. Optionally optimize against a background color, extend an existing palette,
   or adapt colors for protanopia, deuteranopia, and tritanopia.

Palettes update automatically when parameters change. Generation runs in the
browser using the C++ library compiled to WebAssembly.

## Settings and results

The reference illuminant defaults to D65 for displays. D50, D55, A, and E are
also available. Use D50 for print work.

The app displays the palette, analyzes color differences, and previews colors
with simulated color vision deficiency. Copy the palette as JSON, an R vector,
a Python list, or CSS custom properties.

This Markdown page describes the app. To generate and inspect a live palette,
open the interactive app in a browser with JavaScript and WebAssembly enabled.

## Library and documentation

- [C++ library and source code](https://github.com/jolars/qualpal)
- [Official documentation](https://jolars.github.io/qualpal/)
- [R package: qualpalr](https://cran.r-project.org/package=qualpalr)
- [Python package: qualpal](https://pypi.org/project/qualpal/)
- [Author: Johan Larsson](https://jolars.co/)
