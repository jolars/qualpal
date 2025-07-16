# qualpal {#mainpage}

qualpal is a C++ library for automatically generating qualitative color palettes,
maximizing perceptual distinctiveness and supporting color vision deficiency simulation.

## Features

- Perceptually uniform color selection (DIN99d color space)
- Color vision deficiency simulation for accessible palettes
- Flexible builder-style API
- Fast algorithms for large color spaces

## Example

```cpp
#include <qualpal.h>
using namespace qualpal;

// Generate 5 distinct colors from HSL space
Qualpal qp;
qp.setInputColorspace({0, 360}, {0.4, 0.8}, {0.3, 0.7});
auto palette = qp.generate(5);
```

See [Algorithm Documentation](algorithm.md) for details.
