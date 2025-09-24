#!/usr/bin/env bash

set -e

qualpal -n 5 -i colorspace "-170:60" "0:0.7" "0.2:0.8" | \
  python scripts/generate_palette_strip.py docs/images/examples/hsl_pal.svg

qualpal -n 2 -i hex "#ff0000" "#00ff00" "#0000ff" | \
  python scripts/generate_palette_strip.py docs/images/examples/hex_pal.svg

qualpal -n 4 -i palette "ColorBrewer:Set2" --deutan 0.7 | \
  python scripts/generate_palette_strip.py docs/images/examples/brewer_set2_deutan.svg

qualpal -n 5 -i colorspace "0:360" "0.4:0.8" "0.3:0.7" | \
  python scripts/generate_palette_strip.py docs/images/examples/cli_hsl.svg

qualpal -n 3 "#ff0000" "#00ff00" "#0000ff" "#ffff00" | \
  python scripts/generate_palette_strip.py docs/images/examples/cli_hex.svg

qualpal -n 2 --output-delim comma "#ff0000" "#00ff00" "#0000ff" "#ffff00" | \
  python scripts/generate_palette_strip.py docs/images/examples/cli_hex_comma.svg

qualpal -n 2 --colorize never "#1a5577" "#b7e15f" "#6ec8dd" | \
  python scripts/generate_palette_strip.py docs/images/examples/cli_hex_colorized.svg

qualpal -n 3 "#e41a1c" "#377eb8" "#4daf4a" "#984ea3" "#ff7f00" | \
  python scripts/generate_palette_strip.py docs/images/examples/lib_hex.svg

qualpal -n 2 --deutan 0.8 "#e41a1c" "#377eb8" "#4daf4a" "#984ea3" "#ff7f00" | \
  python scripts/generate_palette_strip.py docs/images/examples/lib_hex_deutan.svg

qualpal -n 5 -i colorspace "20:300" "0.3:0.7" "0.7:0.9" | \
  python scripts/generate_palette_strip.py docs/images/examples/lib_pastels.svg

qualpal -n 3 "#f0f0f0" "#e41a1c" "#377eb8" "#4daf4a" -b "#ffffff" | \
  python scripts/generate_palette_strip.py docs/images/examples/lib_bg.svg

qualpal -n 3 -i palette "ColorBrewer:Set2" -b "#000000" --deutan 0.5 --tritan 0.2 | \
  python scripts/generate_palette_strip.py docs/images/examples/lib_cvd.svg

qualpal -n 4 -i palette "Pokemon:Porygon" | \
  python scripts/generate_palette_strip.py docs/images/examples/lib_porygon.svg

qualpal -n 2 --background "#000000" "#e41a1c" "#377eb8" "#4daf4a" "#984ea3" | \
  python scripts/generate_palette_strip.py docs/images/examples/cli_bg_dark.svg

qualpal -n 4 --deutan 0.7 "#e41a1c" "#377eb8" "#4daf4a" "#984ea3" | \
  python scripts/generate_palette_strip.py docs/images/examples/cli_deutan.svg

qualpal -n 5 -i colorspace "-50:200" "0.4:0.8" "0.3:0.7" | \
  python scripts/generate_palette_strip.py docs/images/examples/cli_hsl2.svg

qualpal -n 3 -i colorspace "0:180" "0.3:0.6" "0.7:0.9" --extend "#ff0000" "#00ff00" | \
  python scripts/generate_palette_strip.py docs/images/examples/cli_hsl_extend.svg

qualpal -n 4 -i hex "#4daf4a" "#984ea3" "#ff7f00" "#ffff33" --extend "#e41a1c" "#377eb8" | \
  python scripts/generate_palette_strip.py docs/images/examples/hex_extend.svg

# Paper

## Standard palette
qualpal -n 5 -i colorspace "-190:60" "0:0.7" "0.2:0.8" | \
  python scripts/generate_palette_strip.py paper/images/pal_hsl.pdf

## Palette that's adapted to CVD
qualpal -n 4 -i colorspace "-200:60" "0:0.7" "0.1:0.8" \
  --protan 0.8 --tritan 1.0 | \
  python scripts/generate_palette_strip.py paper/images/pal_cvd.pdf

## From an existing palette, adapted to background
qualpal -n 4 -i palette "Vermeer:PearlEarring"  --background "#ffffff" | \
  python scripts/generate_palette_strip.py paper/images/pal_bg.pdf

## Extended the Bauhaus colors with ones from the Okabe-Ito palette
qualpal -n 4 -i hex "#000000" "#e69f00" "#56b4e9" "#009e73" --extend "#B33A3A" "#2F5DA5" "#E1B84A"  | \
  python scripts/generate_palette_strip.py paper/images/pal_ext.pdf
