#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 2 ]]; then
  echo "Usage: bash tools/logo/update-assets.sh LIGHT_PNG DARK_PNG" >&2
  exit 1
fi

light=$1
dark=$2
magick identify "$light" "$dark" >/dev/null

magick "$light" -resize 320x320 -strip docs/images/logo.png
magick "$dark" -resize 320x320 -strip docs/images/logo-dark.png
magick "$light" -resize 512x512 -strip web/public/logo.png
magick "$light" -resize 96x96 -strip web/public/favicon-96x96.png
magick "$light" -resize 96x96 -define icon:auto-resize=48,32,16 web/public/favicon.ico

for size in 180 192 512; do
  case $size in
    180) icon=apple-touch-icon.png ;;
    *) icon=web-app-manifest-${size}x${size}.png ;;
  esac
  # Keep the whole logo within the safe circle of a maskable icon.
  inner=$((size * 70 / 100))
  magick "$light" -resize "${inner}x${inner}" -background white     -gravity center -extent "${size}x${size}" -alpha remove -alpha off     -strip "web/public/$icon"
done
