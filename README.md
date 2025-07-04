# qualpal <img src='https://raw.githubusercontent.com/jolars/qualpal/refs/heads/main/docs/images/logo.svg' align="right" width="139" />

[![Build Status](https://github.com/user/qualpal/workflows/CI/badge.svg)](https://github.com/user/qualpal/actions)
[![codecov](https://codecov.io/gh/jolars/qualpal/graph/badge.svg?token=p5JTFa9BUz)](https://codecov.io/gh/jolars/qualpal)

A C++ library for generating qualitative color palettes with maximum perceptual distinctiveness, particularly useful for data visualization.

## Features

- **Perceptually uniform color selection** using DIN99d color space
- **Color vision deficiency simulation** for accessible palette design
- **Multiple input formats**: RGB values, hex strings, HSL ranges, built-in palettes
- **Flexible output**: C++ API and command-line interface
- **Fast algorithms** for large color spaces

## Quick Start

### Library Usage

```cpp
#include "qualpal/qualpal.h"

// Generate 5 colors from HSL space
auto colors = qualpal::qualpal(5, {0, 360}, {0.4, 0.8}, {0.3, 0.7});

// Select 3 colors from existing set
std::vector<qualpal::RGB> input = {
    qualpal::RGB("#ff0000"),
    qualpal::RGB("#00ff00"),
    qualpal::RGB("#0000ff")
};
auto selected = qualpal::qualpal(2, input);

// Consider color vision deficiency
std::map<std::string, double> cvd = {{"deuteranomaly", 0.7}};
auto accessible = qualpal::qualpal(4, input, cvd);
```

### Command Line Usage

```bash
# Generate 5 colors from color space
qualpal -n 5 -i colorspace "0:360" "0.4:0.8" "0.3:0.7"

# Select from hex colors
qualpal -n 3 "#ff0000" "#00ff00" "#0000ff" "#ffff00"

# Output in different formats
qualpal -o rgb "#ff0000" "#00ff00"
```

## Installation

### From Source

```bash
git clone https://github.com/user/qualpal.git
cd qualpal
task build
task install
```

### CMake Integration

```cmake
find_package(qualpal REQUIRED)
target_link_libraries(your_target qualpal::qualpal)
```

## Algorithm

qualpal uses the DIN99d color space to measure perceptual color differences and employs a farthest-point sampling algorithm to maximize the minimum distance between selected colors.

## Citation

If you use qualpal in your research, please cite:

```bibtex
@article{qualpal2024,
  title={qualpal: Qualitative Color Palettes in C++},
  author={Your Name},
  journal={Journal of Open Source Software},
  year={2024},
  publisher={The Open Journal}
}
```

## License

MIT License - see [LICENSE](LICENSE) file.

````

## 4. Usage Examples Documentation

[file:docs/examples.md](docs/examples.md) line:1-50
```markdown
# Usage Examples

## Basic Color Selection

```cpp
#include "qualpal/qualpal.h"
#include <iostream>

int main() {
    // Start with some seed colors
    std::vector<qualpal::RGB> colors = {
        qualpal::RGB("#e41a1c"), // Red
        qualpal::RGB("#377eb8"), // Blue
        qualpal::RGB("#4daf4a"), // Green
        qualpal::RGB("#984ea3"), // Purple
        qualpal::RGB("#ff7f00"), // Orange
    };

    // Select 3 most distinct colors
    auto palette = qualpal::qualpal(3, colors);

    for (const auto& color : palette) {
        std::cout << color.hex() << std::endl;
    }

    return 0;
}
````

## Color Vision Deficiency Consideration

```cpp
// Simulate deuteranomaly (red-green colorblindness)
std::map<std::string, double> cvd = {{"deuteranomaly", 1.0}};
auto accessible_palette = qualpal::qualpal(4, colors, cvd);
```

## Custom Color Space Sampling

```cpp
// Generate warm colors: orange to red hues, high saturation, medium lightness
auto warm_colors = qualpal::qualpal(
    6,                    // number of colors
    {15, 45},            // hue range (orange to red-orange)
    {0.7, 1.0},          // high saturation
    {0.4, 0.7}           // medium lightness
);
```

````

## 5. CMake Documentation Integration

[file:CMakeLists.txt](CMakeLists.txt) line:30-45
```cmake
option(BUILD_DOCS "Build documentation" OFF)

if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)

    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()
````
