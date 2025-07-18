---
title: "qualpal: qualitative color palettes for everyone"
tags:
  - C++
  - R
  - data visualization
  - color science
  - optimization
authors:
  - name: Johan Larsson
    orcid: 0000-0002-4029-5945
    affiliation: "1"
affiliations:
  - name: Department of Mathematical Sciences, University of Copenhagen
    index: 1
    ror: 035b05819
date: 17 July 2025
bibliography: paper.bib
---

# Summary

`qualpal` is a C++ library for generating qualitative color palettes with
maximum perceptual distinctiveness, designed for scientific data visualization
and accessibility. It supports flexible palette configuration via a
builder-style API, multiple input formats (RGB, hex, HSL, built-in palettes),
color vision deficiency simulation, and perceptual color difference metrics.
The library uses fast algorithms and perceptually uniform color spaces (DIN99d,
CIEDE2000) to select colors that are maximally distinct, ensuring accessibility
for users with color vision deficiencies.

# Statement of need

Effective visualization of categorical data requires color palettes where
colors are easily distinguishable, including for users with color vision
deficiencies (CVD). Standard palettes are often optimized for a fixed number of
colors and may not be perceptually distinct or accessible. `qualpal` addresses
this gap by providing an automated, reproducible method for generating and
improving color palettes, supporting accessibility and flexible input. It is
useful for researchers, data scientists, and developers who need high-quality,
accessible color palettes for figures, charts, and interfaces.
To support the large community of R users in scientific visualization,
`qualpalr` provides bindings to the C++ library, allowing palette generation
directly from R and integration with R plotting packages.

# Acknowledgements

[Bruce Lindbloom's webpage](http://www.brucelindbloom.com/) has
been a great resource for developing the color classes in qualpal,
and incredibly helpful in debugging color conversions and
ensuring that the color spaces are implemented correctly.

# References
