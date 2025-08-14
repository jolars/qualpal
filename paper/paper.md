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
and accessibility. It supports flexible palette generation via multiple input
formats (RGB, HSL and LCHab (HCL) colorspaces, built-in palettes) and can adapt
palettes to color vision deficiencies (CVD) of the full dichromacy spectrum
(protanopia, deuteranopia, tritanopia) with any choice of severity. The
library is also available as a [R package](https://cran.r-project.org/package=qualpalr)
and as a web application at [qualpal.cc](https://qualpal.cc), providing easy-to-use
interfaces for generating color palettes in R and directly in the browser.

# Statement of need

Effective visualization of categorical data requires color palettes with easily
distinguishable colors, both for people with normal color vision and those with
color vision deficiencies (CVD). `qualpal` addresses this problem by providing
an automatic method for generating and extending color palettes, supporting a
range of inputs and granular control over adaptability to CVD. It is useful for
researchers, data scientists, and developers in need of high-quality,
accessible color palettes for figures, charts, and interfaces.

To support the large community of R users in scientific
visualization, `qualpalr` provides bindings to the C++ library, allowing
palette generation directly from R and integration with R plotting packages.

# Acknowledgements

[Bruce Lindbloom's webpage](http://www.brucelindbloom.com/) has
been a great resource for developing the color classes in qualpal,
and incredibly helpful in debugging color conversions and
ensuring that the color spaces are implemented correctly.

# References
