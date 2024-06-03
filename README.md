# qualpal

[![CI](https://github.com/jolars/qualpal/actions/workflows/ci.yaml/badge.svg)](https://github.com/jolars/qualpal/actions/workflows/ci.yaml)
[![codecov](https://codecov.io/gh/jolars/qualpal/graph/badge.svg?token=p5JTFa9BUz)](https://codecov.io/gh/jolars/qualpal)

**qualpal** generates color palettes for qualitative (categorical) data that are optimized to be as easy to differentiate as possible.

## Bindings

Bindings are not installed by default when running `cmake --install`, even if they have been built. The reason for this is that you typically want to install them as a user, and not system-wide.

### R

To build and install the R bindings, run the following code.

```bash
cmake -B build -S . -DBUILD_R_BINDINGS=ON
cmake --build build
cmake --install build --component R_BINDINGS
```

The package should then be available in R using

```r
library(qualpalr)
```
