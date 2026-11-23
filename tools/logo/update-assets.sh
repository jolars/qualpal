#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 4 ]]; then
  echo "Usage: bash tools/logo/update-assets.sh LIGHT_PNG DARK_PNG SMALL_LIGHT_PNG SMALL_DARK_PNG" >&2
  exit 1
fi

light=$1
dark=$2
small=$3
small_dark=$4
magick identify "$light" "$dark" "$small" "$small_dark" >/dev/null

write_logo() {
  local source=$1 size=$2 output=$3
  local margin=$(((size + 50) / 100))
  local inner=$((size - 2 * margin))
  magick "$source" -trim +repage -resize "${inner}x${inner}" \
    -bordercolor none -border "$margin" -strip "$output"
}

write_logo "$light" 800 docs/images/logo.png
write_logo "$dark" 800 docs/images/logo-dark.png
write_logo "$light" 512 web/public/logo.png
write_logo "$small" 96 docs/images/logo-small.png
write_logo "$small_dark" 96 docs/images/logo-small-dark.png

magick docs/images/logo-small.png -background none -gravity center \
  -extent 96x96 -strip web/public/favicon-96x96.png
magick docs/images/logo-small-dark.png -background none -gravity center \
  -extent 96x96 -strip web/public/favicon-dark-96x96.png

magick web/public/favicon-96x96.png -define icon:auto-resize=48,32,16 web/public/favicon.ico

for size in 180 192 512; do
  case $size in
    180) icon=apple-touch-icon.png ;;
    *) icon=web-app-manifest-${size}x${size}.png ;;
  esac
  # Keep the whole logo within the safe circle of a maskable icon.
  inner=$((size * 70 / 100))
  magick "$small" -trim +repage -resize "${inner}x${inner}" -background white \
    -gravity center -extent "${size}x${size}" -alpha remove -alpha off \
    -strip "web/public/$icon"
done
