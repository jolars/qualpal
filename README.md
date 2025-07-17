# qualpal <img src='https://raw.githubusercontent.com/jolars/qualpal/refs/heads/main/docs/images/logo.svg' align="right" width="139" />

[![Build Status](https://github.com/jolars/qualpal/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/jolars/qualpal/actions/workflows/build-and-test.yml)
[![codecov](https://codecov.io/gh/jolars/qualpal/graph/badge.svg?token=p5JTFa9BUz)](https://codecov.io/gh/jolars/qualpal)

A C++ library for generating qualitative color palettes with maximum perceptual
distinctiveness, especially for data visualization. The API uses a
builder-style interface for flexible palette configuration, supporting multiple
input formats, color vision deficiency simulation, and perceptual color
difference metrics.

## Features

- **Perceptually uniform color selection** using DIN99d color space and other metrics
- **Color vision deficiency simulation** for accessible palette design
- **Multiple input formats**: RGB values, hex strings, HSL ranges, built-in palettes
- **Builder-style C++ API** for flexible palette configuration
- **Fast algorithms** for large color spaces

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

They are also often created by a human designer, which means that they
may not be optimized for perceptual distinctiveness. qualpal, on the other hand,
uses state-of-the art color difference metrics and a farthest-point sampling
algorithm to select colors that are (approximately) maximally distinct from each other, while
also considering color vision deficiencies.

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

### Command Line Usage

```bash
# Generate 5 colors from color space
qualpal -n 5 -i colorspace "0:360" "0.4:0.8" "0.3:0.7"

# Select from hex colors
qualpal -n 3 "#ff0000" "#00ff00" "#0000ff" "#ffff00"

# Generate 5 colors from color space (newline-delimited, default)
qualpal -n 5 -i colorspace "0:360" "0.4:0.8" "0.3:0.7"

# Select from hex colors (space-delimited output)
qualpal -n 3 --output-delim space "#ff0000" "#00ff00" "#0000ff"

# Output comma-delimited palette
qualpal -n 4 --output-delim comma "#ff0000" "#00ff00" "#0000ff" "#ffff00"

# Colorize output (auto-detects terminal by default)
qualpal -n 3 --colorize always "#ff0000" "#00ff00" "#0000ff"

# Pipe palette to analyze subcommand
qualpal -n 5 --output-delim space | xargs qualpal analyze --input hex
```

## Installation

### From Source

To build and install qualpal from source, you need to have CMake and a C++17
compatible C++ compiler installed. If you want the algorithm to be
parallelized, you also need to have support for OpenMP.

To build, clone the repository and run the following commands:

```bash
git clone https://github.com/jolars/qualpal.git
cd qualpal
cmake -B build -S .
cmake --build build --config Release
```

Then you can install it to your system:

```bash
cmake --install build
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

## Contributing

Contributions are welcome! If you have ideas for improvements, bug fixes, or
new features, please open an issue or submit a pull request. For larger
changes, it’s a good idea to discuss your proposal in an issue first.

To contribute code:

1. Fork the repository and create your branch from `main`.
2. Ensure your code is well-tested and passes all existing tests.
3. Run `cmake` and the test suite to verify.
4. Open a pull request with a clear description of your changes.

Thank you for helping make qualpal better!

### Code Style

Please format C++ code using `clang-format` before
submitting a pull request. The project includes a `.clang-format` file
and uses the Mozilla style guide as a base, although
not in terms of naming conventions, where (in short)
we instead use upper CamcelCase for class names,
lower camelCase for function names, and snake_case for variables.

### Commit Guidelines

Use [Conventional Commits](https://www.conventionalcommits.org/) for all commit messages.
Example:

```
feat(cli): add `--output-delim` option
fix: correct color contrast calculation
```

## Versioning

This project follows [Semantic Versioning](https://semver.org/).
Expect breaking changes in major releases, new features
in minor releases, and bug fixes in patch releases.

## License

MIT License - see [LICENSE](LICENSE) file.
