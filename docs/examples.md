# Examples {#examples}

## Library Examples

Generate a palette from HSL color space

```cpp
Qualpal qp;
qp.setInputColorspace({ 0, 360 }, { 0.4, 0.8 }, { 0.3, 0.7 });
auto palette = qp.generate(5);
```

Select colors considering color vision deficiency

```cpp
Qualpal qp;
qp.setInputPalette("ColorBrewer:Set2").setCvd({ { "deutan", 0.7 } });
auto cvd_palette = qp.generate(4);
```

Improve an existing palette for a dark background

```cpp
Qualpal qp;
qp.setInputPalette("ColorBrewer:Set2")
  .setBackground(colors::RGB("#000000"))
  .generate(3);
```

Generate a palette from custom RGB colors

```cpp
Qualpal qp;
qp.setInputRGB({ colors::RGB("#ff0000"),
                 colors::RGB("#00ff00"),
                 colors::RGB("#0000ff"),
                 colors::RGB("#ffff00") });
auto palette = qp.generate(3);
```

Generate a palette using a predefined palette

```cpp
Qualpal qp;
qp.setInputPalette("Pokemon:Porygon");
auto palette = qp.generate(4);
```

Generate a warm color palette (orange to red hues)

```cpp
Qualpal qp;
qp.setInputColorspace({ 15, 45 }, { 0.7, 1.0 }, { 0.4, 0.7 });
auto palette = qp.generate(6);
```

## Command Line Examples {#examples-cli}

Generate 5 colors from HSL color space:

```bash
qualpal -n 5 -i colorspace "0:360" "0.4:0.8" "0.3:0.7"
```

Select 3 colors from hex values:

```bash
qualpal -n 3 "#ff0000" "#00ff00" "#0000ff"
```

Output palette as comma-delimited values:

```bash
qualpal -n 4 --output-delim comma "#ff0000" "#00ff00" "#0000ff" "#ffff00"
```

Colorize output for supported terminals:

```bash
qualpal -n 3 --colorize always "#ff0000" "#00ff00" "#0000ff"
```

Analyze a palette for color difference:

```bash
qualpal analyze --input hex "#ff0000" "#00ff00" "#0000ff"
```

Generate palette for a dark background

```bash
qualpal -n 5 --background "#000000" "#e41a1c" "#377eb8" "#4daf4a" "#984ea3"
```

Simulate color vision deficiency (CVD)

```bash
qualpal -n 4 --cvd deutan:0.7 "#e41a1c" "#377eb8" "#4daf4a" "#984ea3"
```

Improve an existing palette and select subset

```bash
qualpal -n 3 --input-palette "ColorBrewer:Set2" --background "#000000" --cvd tritan:0.2,deutan:0.5
```

Generate warm colors (orange to red hues)

```bash
qualpal -n 6 -i colorspace "15:45" "0.7:1.0" "0.4:0.7"
```
