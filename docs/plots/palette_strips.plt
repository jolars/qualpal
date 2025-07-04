# Generate palette strip visualization
set terminal pngcairo enhanced size 800,200 font
set output 'images/palette_strip.png'

set xrange [0:5]
set yrange [0:1]
set xlabel ""
set ylabel ""
set title "Generated Color Palette"
unset tics
unset border

# Create color rectangles (this requires the selected colors data)
# Note: This is a simplified version - you might need to create specific data for rectangles
plot 'data/selected_colors.dat' using (column(0)):0.5:4 \
     with boxes lc rgb variable fs solid notitle
