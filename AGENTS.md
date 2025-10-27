# Agent Onboarding Guide for qualpal

## Repository Overview

**qualpal** is a C++ library for generating qualitative color palettes with maximum perceptual distinctiveness for data visualization. It includes a core C++ library (builder-style API), CLI, WebAssembly bindings, and a Svelte web app.

**Project Type**: C++ CMake project (~10K LOC)  
**Languages**: C++17, CMake, JavaScript/Svelte, Python (scripts)  
**Key Technologies**: CMake 3.15+, OpenMP, Catch2, CLI11, Emscripten

## Build and Testing Instructions

### Prerequisites

CMake >= 3.15, C++17 compiler (GCC/Clang/MSVC), OpenMP (optional), Catch2 (auto-fetched)

### Essential Build Steps

**ALWAYS configure before building.** Use out-of-source builds in `build/` directory.

**Full build with CLI and tests (recommended):**

```bash
cmake -B build -S . -DBUILD_CLI=ON -DBUILD_TESTING=ON
cmake --build build
```

Build time: ~30-60 seconds

**Run tests (mandatory before submitting):**

```bash
ctest --test-dir build --output-on-failure
```

82 tests, ~2 seconds. All must pass.

**Clean build:**

```bash
rm -rf build && cmake -B build -S . -DBUILD_CLI=ON -DBUILD_TESTING=ON && cmake --build build
```

**Alternative: Use Taskfile**

```bash
task test    # Configure, build with tests, run tests
task build   # Basic build
```

## Project Layout

**Key directories:**

- `include/qualpal/` - Public API headers (qualpal.h is main entry point)
- `src/qualpal/` - Core library implementation
- `src/main.cpp` - CLI application
- `tests/` - Catch2 test suite (82 tests)
- `web/` - Svelte web app (Node.js, Vite)
- `docs/` - Documentation markdown
- `CMakeLists.txt` - Main build config (options: BUILD_CLI, BUILD_TESTING, BUILD_DOCS, BUILD_WASM)

**Architecture:** Builder pattern via `Qualpal` class. Input → Constraints (CVD, background) → Farthest-point sampling → Palette generation.

## CI/CD Workflows

**build-and-test.yml** (PR, push to main) - Primary validation. Tests on Ubuntu, macOS (latest+13), Windows.  
**codecov.yml** (PR, push to main) - Code coverage reporting.  
**leaks.yml** (Manual) - AddressSanitizer/UBSan, Valgrind for memory leaks.  
**docs.yml** (Release, manual) - Doxygen docs to GitHub Pages.  
**release.yml** (Manual) - Semantic versioning releases.

**Pre-commit validation:**

1. Build: `cmake --build build`
2. Test: `ctest --test-dir build --output-on-failure` (all 82 must pass)
3. Format (optional): `clang-format -i <files>`

## Important Patterns and Pitfalls

**Build:** ALWAYS use `cmake -B build -S . [OPTIONS]` then `cmake --build build`. DO NOT run `make` directly.

**CMake Options** (defaults OFF):

- `BUILD_CLI=ON` - CLI application
- `BUILD_TESTING=ON` - Tests (auto-fetches Catch2)
- `BUILD_DOCS=ON` - Docs (needs doxygen, graphviz)
- `BUILD_WASM=ON` - WebAssembly (needs Emscripten)
- `ENABLE_COVERAGE=ON` - Coverage
- `ENABLE_SANITIZER=ON` - AddressSanitizer/UBSan

**Code Style:**

- Formatting: Mozilla style (`.clang-format`), 2-space indent
- Naming: Classes `UpperCamelCase`, functions `lowerCamelCase`, variables `snake_case`
- Commits: Conventional Commits (`feat:`, `fix:`, `docs:`)

**Dependencies:** CLI11 and Catch2 auto-fetched. OpenMP optional (parallelization). help2man optional (man pages).

**Common Issues:**

1. "help2man not found" warning - Ignorable unless packaging
2. "OpenMP not found" - Library works, just slower
3. CLI tests disabled on Windows - Expected behavior
4. First build slow - Fetching CLI11/Catch2

**Web App:** Located in `web/`. Svelte+Vite. Build: `cd web && npm install && npm run build`

## Quick Reference for Common Changes

**C++ library change:**

```bash
# Edit include/qualpal/ or src/qualpal/, add tests in tests/
cmake -B build -S . -DBUILD_TESTING=ON && cmake --build build && ctest --test-dir build --output-on-failure
```

**CLI change:**

```bash
# Edit src/main.cpp, add tests in tests/cli.cpp
cmake -B build -S . -DBUILD_CLI=ON -DBUILD_TESTING=ON && cmake --build build
ctest --test-dir build --output-on-failure
./build/qualpal -n 3 "#ff0000" "#00ff00" "#0000ff"  # Manual test
```

**Documentation:** Edit `docs/*.md` or add Doxygen comments in headers. Verify: `cmake -B build -S . -DBUILD_DOCS=ON && cmake --build build`

**Run specific tests:** `./build/tests "test-name-pattern"`

## Trust These Instructions

These instructions were validated through actual execution. Only search further if you encounter gaps or errors. The build process, test suite, and CI workflows described here are current and working.
