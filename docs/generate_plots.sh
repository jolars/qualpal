#!/bin/bash

echo "Creating data directory..."
mkdir -p ../docs/data ../docs/images

echo "Creating data directory..."
mkdir -p data images ../docs/images

echo "Generating data files..."
./generate_data

echo "Generating HSL sampling plot..."
gnuplot ../docs/plots/hsl_sampling.plt

echo "Generating color space progression plot..."
gnuplot ../docs/plots/color_space_progression.plt

echo "Generating selected colors plot..."
gnuplot ../docs/plots/selected_colors.plt

echo "Generating palette strip..."
gnuplot ../docs/plots/palette_strips.plt

echo "Copying images to docs directory..."
cp -r images/* ../docs/images/

echo "All plots generated successfully!"
echo "Images saved to ../docs/images/"
