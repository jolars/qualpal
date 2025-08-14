# qualpal {#mainpage}

qualpal is a C++ library for automatically generating qualitative color palettes,
maximizing perceptual distinctiveness and supporting color vision deficiency simulation.

**Features**

- Perceptually uniform color selection (DIN99d color space)
- Color vision deficiency simulation for accessible palettes
- Flexible builder-style API
- Fast algorithms for large color spaces

**CLI Interface**

qualpal also provides a command-line interface (CLI) for
generating color palettes from the terminal. This allows users
to quickly generate color palettes without the need for a full
C++ application. Here's a quick example of how to use the CLI:

```bash
qualpal -n 5 -i colorspace "0:360" "0.4:0.8" "0.3:0.7"
```

You can find examples of how to use the CLI
in the [examples](#examples-cli) section.

**Web App**

Qualpal is also available as a web application at
[qualpal.cc](https://qualpal.cc). The web app has slightly
limited functionality compared to the library, but it allows
users to quickly generate color palettes directly in their browser.

**R Package**

The qualpal library is also packaged for R as
[qualpalr](https://cran.r-project.org/package=qualpalr),
which provides a similar API for generating color palettes
from R. This allows users to easily integrate qualpal into their R workflows,
especially for data visualization tasks.

**Next Steps**

- [Get started with the library](#getting-started)
- [Browse the extensive list of examples](#examples)
- [Learn about the algorithm](#algorithm)
- [Explore the color spaces](#colors)
- [Read the API documentation](https://jolars.github.io/qualpal/namespaces.html)
