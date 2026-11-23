# Qualpal 3D logo preview

From the repository root, run:

```sh
cmake -B build -S .
cmake --build build --target generate_logo
build/generate_logo --json > build/logo.json
npm ci --prefix tools/logo
python3 -m http.server --bind 127.0.0.1 8765
```

Open http://127.0.0.1:8765/tools/logo/. Drag to orbit and scroll to zoom.
Use **Download PNG** to save the current view at 1200 × 1200 with a transparent
background. Toggle **Small logo** to preview and export the version with
thicker gray axes and only the selected points. Both versions have light and
dark themes.

To update the shipped assets, reset the camera and download both sizes in
both themes. Run the following from the repository root with
ImageMagick installed.

```sh
bash tools/logo/update-assets.sh qualpal-logo.png qualpal-logo-dark.png \
  qualpal-logo-small.png qualpal-logo-small-dark.png
```
