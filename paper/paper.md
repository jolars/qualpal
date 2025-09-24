---
title: "Qualpal: Qualitative Color Palettes for Everyone"
tags:
  - C++
  - R
  - color palettes
  - data visualization
  - color vision deficiency
  - optimization
authors:
  - name: Johan Larsson
    orcid: 0000-0002-4029-5945
    affiliation: "1"
affiliations:
  - name: Department of Mathematical Sciences, University of Copenhagen
    index: 1
    ror: 035b05819
date: 15 August 2025
bibliography: paper.bib
---

# Summary

Qualpal is a [`C++` library](https://github.com/jolars/qualpal), command-line
tool, [R package](https://cran.r-project.org/package=qualpalr), and [web
app](https://qualpal.cc) for creating qualitative color palettes with maximally
distinct colors. It helps scientists and anyone working with data visualization
choose colors that remain clear and accessible. Qualpal features flexible
palette generation from multiple input formats, such as the HSL and LCH~ab~
color spaces or fixed sets of RGB colors, and can adapt palettes to color
vision deficiencies (CVD) across the full dichromacy spectrum at any severity.
At its core, Qualpal is a lightweight `C++` library with no external
dependencies, making it easy to integrate into other software and programming
languages.

# Statement of need

Effective visualization of categorical data requires color palettes with easily
distinguishable colors---for both people with normal color vision and those
with CVD. Designing a palette is therefore an optimization problem, where the
goal is to maximize the minimum difference among the colors in the palette in
order to make the palette as distinct as possible. This is a non-trivial
problem, since the number of possible palettes grows exponentially with the
number of colors in the palette. And as the number of colors in a palette
_increases_, the minimum distance between colors necessarily _decreases_, since
the colors must be spread out more densely in the color space. As a result, any
given palette can, at best, be optimal only for a particular size. And since
users may also have specific requirements in terms of, for instance, hue,
lightness, saturation, adaptation to CVD, or background color, it is impossible
to provide a set of fixed palettes to cover these needs. Therefore, there is a
need for flexible palette generation tools that can accommodate a wide range of
user requirements and preferences.

This problem has been tackled by, for instance, @glasbey2007, who developed
an algorithm based on simulated annealing that is available in the Python
package Glasbey [@mcinnes2025]. Other tools include iWantHue [@jacomy2025],
Colorgorical [@gramazio2016], distinctipy [@roberts2024], and Palettailor
[@lu2021]. All of these packages rely on some metric to measure the distance
between colors and use some form of optimization algorithm, such as simulated
annealing, to find a set of colors that maximizes the minimum distance between
them in the palette. We summarize these existing packages and their features in
\autoref{tab:packages} and \autoref{tab:packages-features}, respectively.

: Summary of related work and packages, in terms of their algorithms, color
difference metrics, input types, and implementation languages.
\label{tab:packages}

| Package        | Algorithm               | Metrics                  | Input               | Language      |
| -------------- | ----------------------- | ------------------------ | ------------------- | ------------- |
| `Glasbey`      | Simulated annealing     | CIE76                    | LCH~ab~, Fixed      | `Python`      |
| `iWantHue`     | $k$-means, force vector | CIE76                    | LCH~ab~             | `JavaScript`  |
| `Colorgorical` | Random sampling         | CIEDE2000                | LCH~ab~             | `Python`, `C` |
| `distinctipy`  | Random sampling         | L~uv~ approx             | Pastel filter       | `Python`      |
| `Palettailor`  | Simulated annealing     | CIEDE2000                | Hue, lightness      | `JavaScript`  |
| **`Qualpal`**  | Farthest points         | CIEDE2000, DIN99d, CIE76 | HSL, LCH~ab~, Fixed | `C++`         |

All of these existing packages have different strengths and weaknesses. `qualpal` is,
however, the first `C++` library, CLI tool, and R package for generating
qualitative color palettes. It is also the first package to implement a
farthest point sampling algorithm for generating qualitative color palettes,
and the only one to support multiple types of CVD. In addition, it is the only
package to support input from the HSL color space, which represents an
intuitive way to specify colors in terms of hue, saturation, and lightness. It
also supports multiple metrics for measuring color distance, including
CIEDE2000 [@sharma2005] and DIN99d [@cui2002], where the former is
the current standard for color difference advocated by the International Commission
on Illumination (CIE) and the latter is based on Euclidean distances
in the DIN99d color space, which improves upon the CIE76 metric that
uses the CIE~L~a~b~ color space.

: Summary of features of existing packages, in terms of color vision adaptation
(CVD), availability of a web app (Web), command-line interface (CLI), ability
to extend existing palettes, option to adapt to a background color, and
possibility to create palettes with related blocks (such as pairs).
\label{tab:packages-features}

| Package        |     CVD      |       Web        |     CLI      |    Extend    |  Background  |    Blocks    |
| -------------- | :----------: | :--------------: | :----------: | :----------: | :----------: | :----------: |
| `Glasbey`      | $\checkmark$ |                  |              | $\checkmark$ |              | $\checkmark$ |
| `iWantHue`     | $\checkmark$ | $\checkmark$[^1] |              |              |              | $\checkmark$ |
| `Colorgorical` |              | $\checkmark$[^2] |              |              |              |              |
| `distinctipy`  | $\checkmark$ |                  |              |              |              |              |
| `Palettailor`  |              | $\checkmark$[^3] |              |              | $\checkmark$ |              |
| **`Qualpal`**  | $\checkmark$ | $\checkmark$[^4] | $\checkmark$ | $\checkmark$ | $\checkmark$ |              |

[^1]: <https://medialab.github.io/iwanthue/>

[^2]: <http://vrl.cs.brown.edu/color> (but down at the time of writing)

[^3]: <https://iamkecheng.github.io/palettailor/>

[^4]: <https://qualpal.cc>

# Examples

In this section we show some examples of palettes generated with Qualpal. We
begin with a palette generated from candidate colors from part of the HSL color
space, defined by hue in $[0^\circ, 60^\circ]$ and $[170, 360)$^[Note that we
specify `-190` as the starting point in Qualpal to wrap around the hue wheel.],
saturation in $[0, 0.7]$, and lightness in $[0.2, 0.8]$. This produces the
palette shown in \autoref{fig:hsl}. The command to generate this palette is:

```{.sh}
qualpal -n 5 -i colorspace "-190:60" "0:0.7" "0.2:0.8"
```

![A palette of five colors generated from input colors as a subspace of the HSL colorspace.\label{fig:hsl}](images/pal_hsl.pdf)

Next, we show another palette generated from similar input from the HSL
color space. But this time we adapt the palette to the color vision
deficiency types protanomaly (at 80\% severity) and
tritanomaly (at full severity). The resulting palette is shown in \autoref{fig:cvd}. Here,
we show how to generate this palette using the `C++` library interface:

```cpp
#include <qualpal.h>

auto pal = qualpal::Qualpal{}
             .setInputColorspace({ -190, 60 }, { 0, 0.7 }, { 0.2, 0.8 })
             .setCvd({ { "protan", 0.8 }, { "tritan", 1.0 } })
             .generate(4);
```

![A palette of four colors, generated from colors sampled from a portion of the
HSL color space, and adapted to protanomaly and
tritanomaly.\label{fig:cvd}](images/pal_cvd.pdf)

Qualpal also features a set of built-in palettes. In this example, we begin
with a palette derived from Johannes Vermeer's _Girl with a Pearl Earring_, and
pick four colors from it. We also optimize the palette to distinguish it from a
background color of white. The resulting palette is displayed in
\autoref{fig:bg}. This time, we have used the `R` package
[qualpalr](https://cran.r-project.org/package=qualpalr):

```r
library(qualpalr)
pal <- qualpal(4, "Vermeer:PearlEarring", bg = "white")
```

![A palette derived from the colors in Vermeer's *Girl with a Pearl Earring*,
optimized to be distinguished from a white
canvas.\label{fig:bg}](images/pal_bg.pdf)

Finally, we show how to extend an existing palette, in this case one consisting
of three colors inspired by the Bauhaus art movement. We consider a fixed set
of four input colors from the Okabe-Ito palette [@okabe2008] as input. The
result is shown in \autoref{fig:ext} and the CLI command we used to generate
this palette is:

```sh
qualpal -n 4 \
  -i hex "#000000" "#e69f00" "#56b4e9" "#009e73" \
  --extend "#B33A3A" "#2F5DA5" "#E1B84A"
```

![An example of extending an existing palette of three colors (blue, red, and
yellow), with candidates from the Okabe-Ito
palette.\label{fig:ext}](images/pal_ext.pdf)

# Summary of the algorithm

Qualpal begins with a set of input colors. These can be a fixed set of
colors provided by the user, one of the built-in palettes, or a subspace in the
LCH~ab~ or HSL color spaces. In the latter case, we use a quasi-random Halton
sequence [@halton1964] to distribute colors throughout this subspace. The input
colors are then (optionally) projected into a color space corresponding to one
or several CVD types, such as protanopia or deuteranopia, using simulation
methods described by @machado2009.

Next, we compute a full color distance matrix for the colors in the input
set, using the CIEDE2000 [@sharma2005] color difference metric by default.
Finally, we run a farthest point sampling algorithm loosely based on the work
by @schlomer2011, which iteratively swaps colors between a candidate palette
and its complement set until no swap can improve the minimum distance
between colors in the candidate palette. Optionally, a background color can be
included in this step, in which case the palette is optimized to be distinct
from it. The algorithm is deterministic (unlike the other algorithms from
\autoref{tab:packages}) and takes roughly 0.1 seconds to generate a 10-color
palette from a set of 1000 input colors on a modern laptop.

# Acknowledgements

[Bruce Lindbloom's webpage](http://www.brucelindbloom.com/) has been
instrumental in the development of Qualpal, serving as a vital
reference for color space conversions and color difference calculations.

# References
