# Colors

The main functionality of qualpal is to generate qualitative color palettes.
But, in order to do so, it needs to express colors in a variety of different
color spaces. This color handling is exposed through the `qualpal::colors`
namespace in the library, and is able to handle conversion between
several different color spaces, including:

- RGB
- HSL
- Lab
- XYZ
- DIN99d

A chart for conversion between these color spaces is shown below:

@dot
digraph ColorSpaces {
RGB -> HSL;
RGB -> XYZ;
HSL -> RGB;
Lab -> XYZ;
XYZ -> RGB;
XYZ -> Lab;
XYZ -> DIN99d;
}
@enddot

## HSL

The HSL color space is a cylindrical representation of RGB colors, where

- H is the hue, representing the color type (0-360 degrees)
- S is the saturation, representing the intensity of the color (0-1)
- L is the lightness, representing the brightness of the color (0-1)

The HSL color space is popular because it is intuitive and easy to work with,
allowing users to adjust colors by changing the hue, saturation, and lightness,
which can be more intuitive than working with RGB values directly (or other
color spaces).

Let's see how we can create a color in HSL.

```cpp
#include <qualpal.h>

using namespace qualpal::colors;

HSL hsl(120, 0.3, 0.9);
```

## RGB

The RGB color space is a representation of colors using three primary colors:

- R is the red component (0-1)
- G is the green component (0-1)
- B is the blue component (0-1)

Note that we use the 0-1 range for the components rather than the 0-255 range,
which is common in many graphics libraries.

Let's see how we can create a color in RGB.

```cpp
RGB rgb(0.5, 0.2, 0.8);
```

Note that we use the standard RGB space, which is the most common color space
for digital images and displays. It is based on the additive color model, where
the colors are created by combining red, green, and blue light in varying intensities.

## Conversion

Qualpal provides an intuitive API for converting between different color spaces.
To convert a color from the HSL color space to RGB, for instance, we
simply call the constructor of the `RGB` class with an `HSL` object:

```cpp
HSL hsl(120, 0.3, 0.9);
RGB rgb(hsl);
```

Equivalently, we can convert from RGB to HSL:

```cpp
RGB rgb = hsl;
```

Conversions are available between all of the color spaces mentioned above, so
it is for instance possible to convert from RGB directly to Lab.

```cpp
Lab lab{ rgb };
```

## DIN99d

The main purpose of the color classes in qualpal is to
eventually project colors into the DIN99d color space, which is a
perceptually uniform color space. This means that the Euclidean distnace
between two colors in this space corresponds to the perceived difference
between those colors. This is useful for generating qualitative color palettes
that are visually distinct and evenly spaced.

The Lab color space was also designed to be perceptually uniform, but
DIN99d is a more recent development that improves upon Lab by addressing
some of its shortcomings.

We can convert a color to DIN99d using the `DIN99d` class:

```cpp
DIN99d din99d{ rgb };
```

Note, however, that it's not possible to convert back from DIN99d to any
of the other color spaces. This is because DIN99d is designed for
computing distances between colors, and it is not a reversible
transformation.

Technically, DIN99d is actually converted from Lab as an interim step, but
it first uses a modified version of the XYZ color space in which
the Z component of the XYZ color space is scaled, which means that it
cannot be directly converted from Lab.

## White Point Reference

For any color space that attempts to model the human perception of color,
it is necessary to define a white point reference. This is because
the perception of color is dependent on the lighting conditions under which
the colors are viewed. The white point reference defines the color temperature
of the light source, which affects how colors are perceived.
In qualpal, the default white point reference is D65, which is a standard
illuminant that represents average daylight. This is a common choice for
color spaces that are used in digital imaging and displays.

This white point reference is used in the conversion from
XYZ to Lab, and can be set explicitly if needed.

```cpp
XYZ xyz(0.4124564, 0.2126729, 0.0193339);
Lab lab(xyz, std::array<double, 3>{ 0.95047, 1.0, 1.08883 });
```
