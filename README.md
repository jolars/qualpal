# qualpal

[![CI](https://github.com/jolars/qualpal/actions/workflows/ci.yaml/badge.svg)](https://github.com/jolars/qualpal/actions/workflows/ci.yaml)
[![codecov](https://codecov.io/gh/jolars/qualpal/graph/badge.svg?token=p5JTFa9BUz)](https://codecov.io/gh/jolars/qualpal)

**qualpal** generates color palettes for qualitative (categorical) data that are optimized to be as easy to differentiate as possible.

## Command-Line Interface

The command-line interface (CLI) component can be installed by running

```bash
cmake -B build -S . -DBUILD_CLI=ON
cmake --build build
sudo cmake --install build
```
