# qualpal <img src='https://raw.githubusercontent.com/jolars/qualpal/refs/heads/main/docs/images/logo.svg' align="right" width="139" />

[![Build Status](https://github.com/jolars/qualpal/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/jolars/qualpal/actions/workflows/build-and-test.yml)
[![codecov](https://codecov.io/gh/jolars/qualpal/graph/badge.svg?token=p5JTFa9BUz)](https://codecov.io/gh/jolars/qualpal)
[![Docs](https://img.shields.io/badge/docs-stable-blue.svg?logo=readthedocs)](https://jolars.github.io/qualpal/)
[![App](https://img.shields.io/badge/🌐%20%20App-qualpal.cc-blue)](https://qualpal.cc)

A C++ library for generating qualitative color palettes with maximum perceptual
distinctiveness, especially for data visualization. The API uses a
builder-style interface for flexible palette configuration, supporting multiple
input formats, color vision deficiency simulation, and perceptual color
difference metrics.

Full documentation is available at
[https://jolars.github.io/qualpal/](https://jolars.github.io/qualpal/).

This library is also available as an R package called
[qualpalr](https://cran.r-project.org/package=qualpalr), and as a web
application at [qualpal.cc](https://qualpal.cc).

## Features

- **Automatic selection of color palettes using perceptual color difference metrics**
- **Color vision deficiency simulation** for accessible palette design
- **Multiple input formats**: RGB values, hex strings, HSL ranges, built-in palettes
- **Fast algorithms** for large color spaces
- **Builder-style C++ API** for flexible palette configuration
- **Web-Based Interface** for simple and interactive color palette generation

## Table of Contents

- [Features](#features)
- [Motivation](#motivation)
- [R Package](#r-package)
- [Web App](#web-app)
- [Quick Start](#quick-start)
- [Installation](#installation)
- [Usage Examples](#usage-examples)
- [Contributing](#contributing)
- [Versioning](#versioning)
- [License](#license)

## Motivation

When visualizing categorical data, it is often necessary to select a set of
distinct colors that are easily distinguishable from each other. This is
especially important for accessibility, as many people have color vision
deficiencies (CVD) that make it difficult to distinguish certain colors.
qualpal aims to provide a simple and effective way to generate such color
palettes, ensuring that the selected colors are not only distinct from each
other, but also accessible to people with CVD.

The problem with almost all standard color palettes is that they are optimized
for a specific number of colors. This is of course a problem by
design, since a good 10-color palette has to be designed with the assumption
that it will be used as a whole, not that it will be used in parts. But it
means that if you want to select a subset of colors from a palette, you will
not get the best possible result.

They are also often created by a human designer, which means that they may not
be optimized for perceptual distinctiveness. qualpal, on the other hand, uses
state-of-the art color difference metrics and a farthest-point sampling
algorithm to select colors that are (approximately) maximally distinct from
each other, while also considering color vision deficiencies.

## R Package

The qualpal library is also available as an R package called
[qualpalr](https://cran.r-project.org/package=qualpalr). This package
is continuously updated to match the latest features of the C++ library,
but may sometimes lag behind slightly. It provides a similar API for
generating colors from R, making it easy to integrate qualpal into R workflows,

## Web App

Qualpal is also available as a web application at
[qualpal.cc](https://qualpal.cc). The web app has slightly
limited functionality compared to the library and R package, but it allows
users to quickly generate color palettes directly in their browser.

## Quick Start

### Library Usage

```cpp
#include <qualpal.h>

using namespace qualpal;

// Generate 5 colors from HSL colorspace
Qualpal qp;
qp.setInputColorspace({ 0, 360 }, { 0.4, 0.8 }, { 0.3, 0.7 });
auto colorspace_pal = qp.generate(5);

// Select 2 colors from given RGB values
qp.setInputRGB({ colors::RGB("#ff0000"),
                 colors::RGB("#00ff00"),
                 colors::RGB("#0000ff") });
auto rgb_pal = qp.generate(2);

// Consider color vision deficiency (CVD) when generating colors
qp.setInputPalette("ColorBrewer:Set2").setCvd({ { "deutan", 0.7 } });
auto cvd_pal = qp.generate(4);
```

### Command Line Interface (CLI)

qualpal includes a powerful command line interface for palette generation and analysis.

- For a full list of options and usage examples, run:

  ```sh
  qualpal --help
  ```

- There are also man pages available for the CLI:

  ```sh
  man qualpal
  ```

- You can also view the [Doxygen-generated
  documentation](https://jolars.github.io/qualpal/) for detailed CLI and API
  usage.

```sh
# Generate 5 colors from color space
qualpal -n 5 -i colorspace "0:360" "0.4:0.8" "0.3:0.7"

# Select from hex colors
qualpal -n 3 "#ff0000" "#00ff00" "#0000ff" "#ffff00"

# Output comma-delimited palette
qualpal -n 2 --output-delim comma "#ff0000" "#00ff00" "#0000ff" "#ffff00"

# Colorize output (auto-detects terminal by default)
qualpal -n 3 --colorize always "#ff0000" "#00ff00" "#0000ff"

# Analyze palette with hex input
qualpal analyze --input hex "#ffe402" "#ff5733" "#33ff57" "#3357ff"
```

You can read more about the command line interface by calling `qualpal --help` or
by reading the man page at `man qualpal`.

## Installation

### Requirements

- CMake >= 3.15
- C++17 compatible compiler (e.g., GCC, Clang, MSVC)
- [Optional] OpenMP for parallelization
- [Optional] help2man for CLI man page generation
- [Optional] Catch2 for testing

### From Source

To build and install qualpal from source, you need to have CMake and a C++17
compatible C++ compiler installed. If you want the algorithm to be
parallelized, you also need to have support for OpenMP.

To build, clone the repository and run the following commands:

```sh
git clone https://github.com/jolars/qualpal.git
cd qualpal
cmake -B build -S .
cmake --build build --config Release
```

If you want to include the CLI interface, make sure to pass the
`-DBUILD_CLI=ON` option to CMake:

```sh
cmake -B build -S . -DBUILD_CLI=ON
```

Then, you can install it to your system:

```sh
cmake --install build
```

### WebAssembly

To build the WebAssembly package for web applications, you need
to first install [Emscripten](https://emscripten.org/docs/getting_started/downloads.html).

Then, you can build the package with the following commands:

```sh
emcmake cmake -B build/wasm -S . -DBUILD_WASM=ON -DCMAKE_BUILD_TYPE=Release
emmake make -C build/wasm qualpal_wasm
```

### CMake Integration

qualpal can be easily integrated into your CMake projects. After building and
installing the library, you can find it using CMake's `find_package` command.

```cmake
find_package(qualpal REQUIRED)
target_link_libraries(your_target qualpal::qualpal)
```

## Usage Examples

### Basic Color Selection

```cpp
#include <iostream>
#include <qualpal.h>

using namespace qualpal;

int
main()
{
  // Start with some seed colors
  Qualpal qp;
  qp.setInputRGB({
    colors::RGB("#e41a1c"), // Red
    colors::RGB("#377eb8"), // Blue
    colors::RGB("#4daf4a"), // Green
    colors::RGB("#984ea3"), // Purple
    colors::RGB("#ff7f00"), // Orange
  });

  // Select 3 most distinct colors
  auto palette = qp.generate(3);

  for (const auto& color : palette) {
    std::cout << color.hex() << std::endl;
  }

  return 0;
}
```

### Color Vision Deficiency Consideration

Simulate deuteranomaly (red-green colorblindness) of severity 0.8.

```cpp
Qualpal qp;
qp.setInputRGB(colors).setCvd({ { "deutan", 0.8 } });
auto accessible_palette = qp.generate(4);
```

### Custom Color Space Sampling

Generate warm colors: orange to red hues, high saturation, medium lightness

```cpp
Qualpal qp;
qp.setInputColorspace({ 15, 45 }, { 0.7, 1.0 }, { 0.4, 0.7 });
auto warm_colors = qp.generate(6);
```

### Cater to Background Colors

When visualizing categorical data, consider a background color to ensure contrast.

```cpp
auto pal = Qualpal{}
             .setInputRGB({
               colors::RGB("#f0f0f0"), // Light color (which we want to avoid)
               colors::RGB("#e41a1c"), // Red
               colors::RGB("#377eb8"), // Blue
               colors::RGB("#4daf4a"), // Green
             })
             .setBackgroundColor(colors::RGB("#ffffff"))
             .generate(3);
```

### Improve Existing Palettes

Qualpal can take an existing color palette and improve it by selecting a subset
of the most distinct colors from it, as well as ordering the result.

Here we design a CVD friendly version of the `"ColorBrewer:Set2"` palette
for a black background, selecting 3 colors.

```cpp
auto pal = Qualpal{}
             .setInputPalette("ColorBrewer:Set2")
             .setBackgroundColor(colors::RGB("#000000"))
             .setCvd({ { "tritan", 0.2 }, { "deutan", 0.5 } })
             .generate(3);
```

### Palette Extension Example

You can extend an existing palette by adding more distinct colors to it:

```cpp
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

// Extend to 6 colors, keeping the first two fixed
auto ext_pal = Qualpal{}.setInputRGB(input).extend(fixed, 4);
```

## Contributing

Contributions are welcome! Please see the [CONTRIBUTING](CONTRIBUTING.md) file
for more information.

## Versioning

This project follows [Semantic Versioning](https://semver.org/).
Expect breaking changes in major releases, new features
in minor releases, and bug fixes in patch releases.

## License

MIT License - see [LICENSE](LICENSE) file.
